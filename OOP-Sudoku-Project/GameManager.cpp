// GameManager.cpp
// Implementation of GameManager class
// Handles user interface, game state management, and file operations

#include "GameManager.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <cstdlib>

const std::string MARGIN = "                              ";

GameManager::GameManager() : currentGame(nullptr) {
}

void GameManager::clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void GameManager::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int GameManager::getDifficultyLevel() {
    int level;
    clearScreen();
    std::cout << "\n\n\n" << MARGIN << "\033[1;33m==== SELECT DIFFICULTY ====\033[0m\n";
    std::cout << MARGIN << "\033[1;36m [1] Easy (30 empty cells)\033[0m\n";
    std::cout << MARGIN << "\033[1;36m [2] Medium (40 empty cells)\033[0m\n";
    std::cout << MARGIN << "\033[1;36m [3] Hard (50 empty cells)\033[0m\n";
    std::cout << MARGIN << "\033[1;33m===========================\033[0m\n";
    std::cout << MARGIN << "Selection: ";
    
    while (true) {
        std::cin >> level;
        
        if (std::cin.fail()) {
            clearInputBuffer();
            std::cout << MARGIN << "\033[1;31mInvalid input. Please enter 1, 2, or 3: \033[0m";
            continue;
        }
        
        clearInputBuffer();
        
        if (level >= 1 && level <= 3) {
            return level;
        }
        
        std::cout << MARGIN << "\033[1;31mInvalid choice. Please enter 1, 2, or 3: \033[0m";
    }
}

bool GameManager::getMove(int& row, int& col, int& value) {
    std::cout << "\n" << MARGIN << "\033[1;33mCmd:\033[0m 'set A3 9' | 'save' | 'exit'\n";
    std::cout << MARGIN << "\033[1;33mInput:\033[0m ";
    
    std::string cmd;
    std::cin >> cmd;
    
    if (std::cin.fail()) {
        clearInputBuffer();
        return false;
    }
    
    if (cmd == "exit") {
        clearInputBuffer();
        row = -1;
        col = -1;
        value = 0;
        return true;
    }
    
    if (cmd == "save") {
        clearInputBuffer();
        row = -2;
        col = -2;
        value = 0;
        return true;
    }
    
    if (cmd == "set") {
        std::string pos;
        std::cin >> pos >> value;
        
        if (std::cin.fail() || pos.length() < 2) {
            clearInputBuffer();
            std::cout << MARGIN << "\033[1;31mInvalid format. Use: set A3 9\033[0m\n";
            std::cout << MARGIN << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return false;
        }
        
        clearInputBuffer();
        
        row = toupper(pos[0]) - 'A';
        col = pos[1] - '1';
        
        return true;
    }
    
    clearInputBuffer();
    std::cout << MARGIN << "\033[1;31mUnknown command. Use 'set', 'save', or 'exit'\033[0m\n";
    std::cout << MARGIN << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return false;
}

void GameManager::playGameLoop() {
    if (!currentGame) {
        std::cout << MARGIN << "\033[1;31mNo active game!\033[0m\n";
        return;
    }
    
    bool playing = true;
    
    while (playing) {
        clearScreen();
        currentGame->display();
        
        if (currentGame->isComplete()) {
            std::cout << "\n\n" << MARGIN << "\033[1;32m*** CONGRATULATIONS! You solved the puzzle! ***\033[0m\n";
            std::cout << MARGIN << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            currentGame.reset();
            return;
        }
        
        int row, col, value;
        if (!getMove(row, col, value)) {
            continue;
        }
        
        if (row == -1 && col == -1) {
            char saveChoice;
            std::cout << "\n" << MARGIN << "\033[1;33mSave game before quitting? (y/n): \033[0m";
            std::cin >> saveChoice;
            clearInputBuffer();
            
            if (saveChoice == 'y' || saveChoice == 'Y') {
                if (currentGame->saveToFile(SAVE_FILE)) {
                    std::cout << MARGIN << "\033[1;32mGame saved successfully!\033[0m\n";
                } else {
                    std::cout << MARGIN << "\033[1;31mFailed to save game.\033[0m\n";
                }
                std::cout << MARGIN << "Press Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            
            currentGame.reset();
            playing = false;
            continue;
        }
        
        if (row == -2 && col == -2) {
            if (currentGame->saveToFile(SAVE_FILE)) {
                std::cout << "\n" << MARGIN << "\033[1;32mGame saved successfully!\033[0m\n";
            } else {
                std::cout << "\n" << MARGIN << "\033[1;31mFailed to save game.\033[0m\n";
            }
            std::cout << MARGIN << "Press Enter to continue...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        
        currentGame->makeMove(row, col, value);
        std::cout << MARGIN << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void GameManager::playNewGame() {
    int difficulty = getDifficultyLevel();
    
    clearScreen();
    std::cout << "\n\n\n" << MARGIN << "\033[1;36mGenerating new Sudoku board...\033[0m\n";
    currentGame = std::make_unique<SudokuBoard>(difficulty);
    
    std::cout << MARGIN << "\033[1;32mBoard generated! Good luck!\033[0m\n";
    std::cout << "\n" << MARGIN << "\033[1;33mInstructions:\033[0m\n";
    std::cout << MARGIN << "- Enter moves as: \033[1;37mset A3 5\033[0m (row, col, value)\n";
    std::cout << MARGIN << "- Type '\033[1;37msave\033[0m' to save current game\n";
    std::cout << MARGIN << "- Type '\033[1;37mexit\033[0m' to quit\n";
    std::cout << "\n" << MARGIN << "Press Enter to start...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    playGameLoop();
}

bool GameManager::saveExists() const {
    std::ifstream file(SAVE_FILE);
    return file.good();
}

void GameManager::loadGame() {
    if (!saveExists()) {
        clearScreen();
        std::cout << "\n\n\n" << MARGIN << "\033[1;31mNo saved game found.\033[0m\n";
        std::cout << MARGIN << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    
    currentGame = std::make_unique<SudokuBoard>();
    
    if (currentGame->loadFromFile(SAVE_FILE)) {
        clearScreen();
        std::cout << "\n\n\n" << MARGIN << "\033[1;32mGame loaded successfully!\033[0m\n";
        std::cout << "\n" << MARGIN << "\033[1;33mInstructions:\033[0m\n";
        std::cout << MARGIN << "- Enter moves as: \033[1;37mset A3 5\033[0m (row, col, value)\n";
        std::cout << MARGIN << "- Type '\033[1;37msave\033[0m' to save current game\n";
        std::cout << MARGIN << "- Type '\033[1;37mexit\033[0m' to quit\n";
        std::cout << "\n" << MARGIN << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        playGameLoop();
    } else {
        clearScreen();
        std::cout << "\n\n\n" << MARGIN << "\033[1;31mFailed to load game.\033[0m\n";
        std::cout << MARGIN << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        currentGame.reset();
    }
}

void GameManager::deleteSave() {
    if (!saveExists()) {
        clearScreen();
        std::cout << "\n\n\n" << MARGIN << "\033[1;31mNo saved game found to delete.\033[0m\n";
        std::cout << MARGIN << "Press Enter to continue...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    
    clearScreen();
    char confirm;
    std::cout << "\n\n\n" << MARGIN << "\033[1;33mAre you sure you want to delete the saved game? (y/n): \033[0m";
    std::cin >> confirm;
    clearInputBuffer();
    
    if (confirm == 'y' || confirm == 'Y') {
        if (std::remove(SAVE_FILE) == 0) {
            std::cout << "\n" << MARGIN << "\033[1;32mSaved game deleted successfully.\033[0m\n";
        } else {
            std::cout << "\n" << MARGIN << "\033[1;31mFailed to delete saved game.\033[0m\n";
        }
    } else {
        std::cout << "\n" << MARGIN << "\033[1;36mDelete cancelled.\033[0m\n";
    }
    std::cout << MARGIN << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}