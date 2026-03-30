// SudokuBoard.h
// Represents a Sudoku board with validation and solving capabilities
// Contains the game state and operations for a single Sudoku puzzle

#ifndef SUDOKU_BOARD_H
#define SUDOKU_BOARD_H

#include <vector>
#include <string>

const int BOARD_SIZE = 9;
const int SUBGRID_SIZE = 3;
const int EMPTY_CELL = 0;

class SudokuBoard {
private:
    std::vector<std::vector<int>> board;
    std::vector<std::vector<bool>> fixed;
    int difficulty;
    
    bool isValidPlacement(int row, int col, int num) const;
    bool solveSudoku(int row, int col);
    void generateSolution();
    void removeNumbers(int count);

public:
    SudokuBoard();
    explicit SudokuBoard(int difficultyLevel);
    ~SudokuBoard() = default;
    
    void generateNewBoard(int difficultyLevel);
    void display() const;
    bool makeMove(int row, int col, int value);
    bool isComplete() const;
    bool isValid() const;
    
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);
    
    int getCell(int row, int col) const;
    bool isCellFixed(int row, int col) const;
    int getDifficulty() const { return difficulty; }
};

#endif