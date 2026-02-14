#include "MinesweeperGame.h"
#include <iostream> //to print out error messages
#include <ctime> //used to randomize mine placement
#include <iomanip> //input output manipulation, used to format the timer display
#include <sstream> //takes numbers and converts them to a string that can be drawn
#include <fstream> //reads or write files

MinesweeperGame::MinesweeperGame() {
    window.create(sf::VideoMode({COLUMNS * CELLSIZE, (ROWS + 1) * CELLSIZE}), "MINESWEEPER GAME");
    //Asset Loading with proper error handling
    sf::Image icon;
    if (icon.loadFromFile("assets/mine.png")){
        window.setIcon(icon.getSize(), icon.getPixelsPtr());}
    else{
        std::cerr << "Error: Couldn't load window icon"<<std::endl;
    }
    //Lambda functions used to ease asset loading
    auto loadTextures = [&](std::string key, std::string path) {
        return textures[key].loadFromFile(path);
    };
    auto loadSound = [&](std::string key, std::string path) {
        return buffers[key].loadFromFile(path);
    };

    if (!loadTextures("spritesheet","assets/spritesheet.jpg") || !loadTextures("faces","assets/faces.jpg") ||
        !loadSound("click", "audio/mouse_click.mp3") || !loadSound("explosion", "audio/medium_explosion.mp3") ||
        !loadSound("win", "audio/victory.mp3") || !loadSound("lose", "audio/game_over.mp3")
        || !font.openFromFile("fonts/Digital Display.ttf")) {
        std::cerr << "Failed to load asset!" << std::endl;
        exit(EXIT_FAILURE);
    }
    //Initializing sound objects using the buffers loaded above
    clickSound.emplace(buffers["click"]);
    explodeSound.emplace(buffers["explosion"]);
    winSound.emplace(buffers["win"]);
    lossSound.emplace(buffers["lose"]);
    //Reads best time file
    std::ifstream inputFile("best_time.txt");
    if (inputFile.is_open()) {
        inputFile >> bestTime;
        inputFile.close();
    } else {
        bestTime = 9999;}

    resetBoard();
}
//Calculates size and coordinates of the spritesheet using tile index
sf::IntRect MinesweeperGame::getTextureRect(int index) const {
    const int tileWidth = 60;  
    const int tileHeight = 60; 
    const int columnsInSheet = 4;
    int x = (index % columnsInSheet) * tileWidth;
    int y = (index / columnsInSheet) * tileHeight;
    return sf::IntRect({x, y}, {tileWidth, tileHeight});
}

//Calculates size and coordinates of the faces spritesheet using tile index
sf::IntRect MinesweeperGame::getFacesRect(int index) const {
    const int tileWidth = 60;  
    const int tileHeight = 60; 
    const int columnsInSheet = 5;
    int x = (index % columnsInSheet) * tileWidth;
    int y = (index / columnsInSheet) * tileHeight;
    return sf::IntRect({x, y}, {tileWidth, tileHeight});
}
//Determines placement of Mines and calculates Neighbours
void MinesweeperGame::resetBoard() {
    grid.assign(ROWS, std::vector<Cell>(COLUMNS));
    //Picks a random position and checks if there's already a mine there. If there isnt, one is placed
    int placed = 0;
    while (placed < MINECOUNT) {
        int x = std::rand() % COLUMNS;
        int y = std::rand() % ROWS;
        if (grid[y][x].isMine==false) {
            grid[y][x].isMine = true;
            placed++;
        }
    }
    // Calculates the number of mines next to a non mine cell
    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLUMNS; ++x) {
            if (grid[y][x].isMine) continue;
            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {
                    int nx = x + dx, ny = y + dy;
                    if (nx >= 0 && nx < COLUMNS && ny >= 0 && ny < ROWS && grid[ny][nx].isMine)
                        grid[y][x].neighborCount++;
                }
            }
        }
    }
}
//Automatically reveals all the safe empty cells after an empty cell has been revealed
void MinesweeperGame::revealEmpty(int x, int y) {
    if (x < 0 || x >= COLUMNS|| y < 0 || y >= ROWS || grid[y][x].isRevealed || grid[y][x].isFlagged) return;
    
    grid[y][x].isRevealed = true;
    if (grid[y][x].neighborCount == 0 && !grid[y][x].isMine) {
        for (int dy = -1; dy <= 1; ++dy)
            for (int dx = -1; dx <= 1; ++dx)
                revealEmpty(x + dx, y + dy);
    }
}
//Reveals all unflagged mines after loss
void MinesweeperGame::revealAllMines() {
    for (auto& row : grid){
        for (auto& cell : row){
            if (cell.isMine){
                if(cell.isFlagged==false){
                cell.isRevealed = true;}}
            else if(cell.isFlagged){
                cell.isFlagged = false;} 
        }
    }
}
//Handles User input and triggers Game Over or Victory states
void MinesweeperGame::handleMouse(sf::Mouse::Button button) {
    sf::Vector2i position = sf::Mouse::getPosition(window);
    int gx = position.x / (int)CELLSIZE;
    int gy = (position.y/(int)CELLSIZE) - 1;
    //Restarts the game if face is clicked
    if (button == sf::Mouse::Button::Left && position.y < (int)CELLSIZE && gx == 4) {
        clickSound->play();
        isGameOver = hasWon = false;
        gameClock.restart();
        resetBoard();
        return;
    }
    //Ignores any click on the board if any of the conditions are true
    if (isGameOver || hasWon || gy < 0 || gy >= ROWS || gx < 0 || gx >= COLUMNS) {
        return;}
    //Reveals a left-clicked cell
    if (button == sf::Mouse::Button::Left) {
        if (grid[gy][gx].isFlagged) {
            return; 
        }
        if (grid[gy][gx].isMine) {
            isGameOver = true;
            revealAllMines();
            explodeSound->play();
            lossSound->play();
        } 
        else {
            revealEmpty(gx, gy);
            checkWin();
        }
    } 
    //Flags a right-clicked cell
    else if (button == sf::Mouse::Button::Right) {
        grid[gy][gx].isFlagged = !grid[gy][gx].isFlagged;
    }
}
//Checks win conditions
void MinesweeperGame::checkWin() {
    int hidden = 0;
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            if (cell.isRevealed==false) hidden++;
        }
    }
    if (hidden == MINECOUNT) {
        hasWon = true;
        winSound->play();

        int currentScore = (int)gameClock.getElapsedTime().asSeconds();
        if (currentScore < bestTime) {
            bestTime = currentScore;
            std::ofstream outputFile("best_time.txt");
            if (outputFile.is_open()) {
                outputFile << bestTime;
                outputFile.close();}
        }

        for (auto& row : grid) {
            for (auto& cell : row) {
                if (cell.isMine) {cell.isFlagged = true;}
            }
        }
    }
}

void MinesweeperGame::run() {
    //Keeps the game running as long as the window is open
    while (window.isOpen()) {
        processEvents();
        updateTimer();
        updateBestTime();
        render();
    }
}
//Constantly checks what the user is doing and handles it
void MinesweeperGame::processEvents() {
    while (const std::optional<sf::Event> event = window.pollEvent()){
        if (event->is<sf::Event::Closed>()){
            window.close();}
        if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
            handleMouse(mouse->button);
        }
    }
}
//Calculates time elapsed and formats it using stringstream
void MinesweeperGame::updateTimer(){
    int totalSeconds;
    if (!isGameOver && !hasWon) {
        totalSeconds = (int)gameClock.getElapsedTime().asSeconds();
        finalTime = (float)totalSeconds;} 
    else {
        totalSeconds = (int)finalTime;}

    int minutes = totalSeconds/60;
    int seconds = totalSeconds % 60;

    std::stringstream stringStream;
    stringStream<<std::setfill('0')<<std::setw(2)<<minutes<<":"
    <<std::setfill('0')<<std::setw(2)<<seconds;
    formattedTime = stringStream.str();
}
//Updates best time
void MinesweeperGame::updateBestTime(){
    std::stringstream bestStringStream;
    if (bestTime >= 9999) {
        bestStringStream << "Best: --:--";
    } 
    else {
        int bestMinutes = bestTime / 60;
        int bestSeconds = bestTime % 60;
        bestStringStream << "BEST: " << std::setfill('0') << std::setw(2) << bestMinutes 
               << ":" << std::setfill('0') << std::setw(2) << bestSeconds;
    }
    formattedBestTime = bestStringStream.str();
}
//Draws background, User Interface and grid state
void MinesweeperGame::render() {
    window.clear(sf::Color::Black); //Black background

    sf::Text timerText(font, formattedTime,60);
    timerText.setFillColor(sf::Color::Red);
    timerText.setPosition({(float)(COLUMNS * CELLSIZE) - 160, -6});
    window.draw(timerText); 

    sf::Text bestText(font, formattedBestTime, 30); 
    bestText.setFillColor(sf::Color::Yellow); 
    bestText.setPosition({10, 10});
    window.draw(bestText);

    sf::Sprite faces(textures["faces"]);
    faces.setPosition({(float)4 * CELLSIZE, 0});
    //Checks the game status to determine what face from the faces spritesheet should be drawn
    int faceIndex = 0;
    if (hasWon) faceIndex = 4;
    else if (isGameOver) faceIndex = 2;

    faces.setTextureRect(getFacesRect(faceIndex));
    window.draw(faces);

    for (int y = 0; y < ROWS; ++y) {
        for (int x = 0; x < COLUMNS; ++x) {
            const Cell& cell = grid[y][x];
            sf::Sprite sprite(textures["spritesheet"]); 
            
            sprite.setPosition({(float)x * CELLSIZE, (float)(y + 1) * CELLSIZE});

            if (cell.isRevealed==false) {
                if (cell.isFlagged) {
                    sprite.setTextureRect(getTextureRect(1));} //draws flag 
                else {
                    sprite.setTextureRect(getTextureRect(0));} //draws button
            } 
            else {
                if (cell.isMine) {
                    sprite.setTextureRect(getTextureRect(2));} //draws mine
                else {
                    //calculates the neighbouring mines and draws the corresponding empty/ numbered tile under the buttons
                    int index;
                    if (cell.neighborCount == 0) {
                        index = 3;
                    }
                    else{ index =(3 + cell.neighborCount);}
                    sprite.setTextureRect(getTextureRect(index));}
            }
            window.draw(sprite);
        }
    }
    window.display();
}
