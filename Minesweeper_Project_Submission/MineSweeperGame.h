#ifndef MINESWEEPER_GAME_H
#define MINESWEEPER_GAME_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector> //used to create the game board
#include <map> //used to assign keys to assets 
#include <string> 
#include <optional> //used for the sound objects
#include "Cell.h"

//The Central class to manage game mechanics
class MinesweeperGame {
public:
    MinesweeperGame(); //constructor
    void run(); //the main game loop

private:
    // Core Functionality Methods
    void processEvents(); //Handles window closing and input polling
    void handleMouse(sf::Mouse::Button button); //Handles the logic for left and right click
    void updateLogic(); //Updates game state
    void render(); //Handles all the drawing calls
    void revealAllMines(); //Shows the location of all mines upon loss/game over
    void resetBoard(); //Clears grid and places mines randomly again
    void revealEmpty(int x, int y); //Triggers flood reveal algorithm
    void checkWin(); //Checks if all the cells without mines are revealed
    
    // User Interface and Data Formatting
    void updateTimer(); //Changes the current game time into an MM::SS format
    void updateBestTime(); //Formats best time record for display
    sf::IntRect getTextureRect(int index) const; // Cuts the tile spritesheet
    sf::IntRect getFacesRect(int index) const; //Cuts the faces spritesheet

    //Data Members
    std::string formattedTime; // String for current timer
    std::string formattedBestTime; //String for best time
    sf::RenderWindow window; // Main window object 
    std::vector<std::vector<Cell>> grid; //2 Dimensional vector representing the game board
    std::map<std::string, sf::Texture> textures,spritesheet; //Management map for the textures
    std::map<std::string, sf::SoundBuffer> buffers; // Management map for the audio buffers
    
    std::optional<sf::Sound> clickSound, explodeSound, winSound, lossSound;
    sf::Font font; //The font for the timer and best time
    sf::Clock gameClock; //Tracks the time elapsed
    int bestTime = 9999; //Value loaded from best_time.txt
    float finalTime=0; //Snap-shot of time at moment of win/loss

    bool isGameOver{false}; // Checks for loss
    bool hasWon{false}; // Checks for win

    // GAME CONSTANTS
    const unsigned int CELLSIZE{60};
    const int ROWS{9};
    const int COLUMNS{9};
    const int MINECOUNT{10};
};

#endif