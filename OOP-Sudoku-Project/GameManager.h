// GameManager.h
// Manages game flow, user interactions, and save/load operations
// Coordinates between user input and the SudokuBoard

#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "SudokuBoard.h"
#include <string>
#include <memory>

class GameManager {
private:
    std::unique_ptr<SudokuBoard> currentGame;
    static constexpr const char* SAVE_FILE = "sudoku_save.txt";
    
    int getDifficultyLevel();
    void playGameLoop();
    bool getMove(int& row, int& col, int& value);
    void clearInputBuffer();
    void clearScreen();

public:
    GameManager();
    ~GameManager() = default;
    
    void playNewGame();
    void loadGame();
    void deleteSave();
    bool saveExists() const;
};

#endif