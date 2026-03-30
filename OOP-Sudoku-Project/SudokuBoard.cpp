// SudokuBoard.cpp
// Implementation of the SudokuBoard class
// Handles board generation, validation, and file I/O operations

#include "SudokuBoard.h"
#include <iostream>
#include <fstream>
#include <random>
#include <algorithm>
#include <ctime>

SudokuBoard::SudokuBoard() 
    : board(BOARD_SIZE, std::vector<int>(BOARD_SIZE, EMPTY_CELL)),
      fixed(BOARD_SIZE, std::vector<bool>(BOARD_SIZE, false)),
      difficulty(1) {
}

SudokuBoard::SudokuBoard(int difficultyLevel)
    : board(BOARD_SIZE, std::vector<int>(BOARD_SIZE, EMPTY_CELL)),
      fixed(BOARD_SIZE, std::vector<bool>(BOARD_SIZE, false)),
      difficulty(difficultyLevel) {
    generateNewBoard(difficultyLevel);
}

bool SudokuBoard::isValidPlacement(int row, int col, int num) const {
    if (num < 1 || num > 9) return false;
    
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (board[row][i] == num && i != col) return false;
        if (board[i][col] == num && i != row) return false;
    }
    
    int boxRow = (row / SUBGRID_SIZE) * SUBGRID_SIZE;
    int boxCol = (col / SUBGRID_SIZE) * SUBGRID_SIZE;
    
    for (int i = boxRow; i < boxRow + SUBGRID_SIZE; ++i) {
        for (int j = boxCol; j < boxCol + SUBGRID_SIZE; ++j) {
            if (board[i][j] == num && (i != row || j != col)) {
                return false;
            }
        }
    }
    
    return true;
}

bool SudokuBoard::solveSudoku(int row, int col) {
    if (row == BOARD_SIZE) return true;
    if (col == BOARD_SIZE) return solveSudoku(row + 1, 0);
    if (board[row][col] != EMPTY_CELL) return solveSudoku(row, col + 1);
    
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::shuffle(numbers.begin(), numbers.end(), 
                 std::default_random_engine(static_cast<unsigned>(std::time(nullptr)) + row * 9 + col));
    
    for (int num : numbers) {
        if (isValidPlacement(row, col, num)) {
            board[row][col] = num;
            if (solveSudoku(row, col + 1)) return true;
            board[row][col] = EMPTY_CELL;
        }
    }
    
    return false;
}

void SudokuBoard::generateSolution() {
    for (auto& row : board) {
        std::fill(row.begin(), row.end(), EMPTY_CELL);
    }
    solveSudoku(0, 0);
}

void SudokuBoard::removeNumbers(int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, BOARD_SIZE - 1);
    
    int removed = 0;
    while (removed < count) {
        int row = dis(gen);
        int col = dis(gen);
        
        if (board[row][col] != EMPTY_CELL) {
            board[row][col] = EMPTY_CELL;
            fixed[row][col] = false;
            ++removed;
        }
    }
}

void SudokuBoard::generateNewBoard(int difficultyLevel) {
    difficulty = difficultyLevel;
    generateSolution();
    
    for (auto& row : fixed) {
        std::fill(row.begin(), row.end(), true);
    }
    
    int cellsToRemove = 30 + (difficultyLevel * 10);
    if (cellsToRemove > 60) cellsToRemove = 60;
    
    removeNumbers(cellsToRemove);
}

void SudokuBoard::display() const {
    const std::string MARGIN = "                              ";
    
    std::cout << "\n\n" << MARGIN << "\033[38;2;150;150;150m    1 2 3   4 5 6   7 8 9\033[0m\n";
    
    for (int i = 0; i < BOARD_SIZE; ++i) {
        if (i == 3 || i == 6) {
            std::cout << MARGIN << "\033[38;2;150;150;150m    ------+-------+------\033[0m\n";
        }
        
        std::cout << MARGIN << "\033[38;2;150;150;150m" << static_cast<char>('A' + i) << "   \033[0m";
        
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (j == 3 || j == 6) {
                std::cout << "\033[38;2;150;150;150m| \033[0m";
            }
            
            if (board[i][j] == EMPTY_CELL) {
                std::cout << ". ";
            } else if (fixed[i][j]) {
                // Fixed cells (original puzzle) in green
                std::cout << "\033[32m" << board[i][j] << " \033[0m";
            } else {
                // User-entered cells in bright white
                std::cout << "\033[1;37m" << board[i][j] << " \033[0m";
            }
        }
        std::cout << "\n";
    }
}

bool SudokuBoard::makeMove(int row, int col, int value) {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        const std::string MARGIN = "                              ";
        std::cout << "\n" << MARGIN << "\033[1;31mInvalid position! Row must be A-I and column must be 1-9.\033[0m\n";
        return false;
    }
    
    if (fixed[row][col]) {
        const std::string MARGIN = "                              ";
        std::cout << "\n" << MARGIN << "\033[1;31mCannot modify a fixed cell!\033[0m\n";
        return false;
    }
    
    if (value < 0 || value > 9) {
        const std::string MARGIN = "                              ";
        std::cout << "\n" << MARGIN << "\033[1;31mInvalid value! Must be 0-9 (0 to clear).\033[0m\n";
        return false;
    }
    
    if (value == 0) {
        board[row][col] = EMPTY_CELL;
        return true;
    }
    
    if (!isValidPlacement(row, col, value)) {
        const std::string MARGIN = "                              ";
        std::cout << "\n" << MARGIN << "\033[1;31mInvalid move! This number conflicts with existing numbers.\033[0m\n";
        return false;
    }
    
    board[row][col] = value;
    return true;
}

bool SudokuBoard::isComplete() const {
    for (const auto& row : board) {
        for (int cell : row) {
            if (cell == EMPTY_CELL) return false;
        }
    }
    return isValid();
}

bool SudokuBoard::isValid() const {
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            if (board[i][j] != EMPTY_CELL) {
                int val = board[i][j];
                for (int k = 0; k < BOARD_SIZE; ++k) {
                    if (k != j && board[i][k] == val) return false;
                    if (k != i && board[k][j] == val) return false;
                }
                
                int boxRow = (i / SUBGRID_SIZE) * SUBGRID_SIZE;
                int boxCol = (j / SUBGRID_SIZE) * SUBGRID_SIZE;
                for (int r = boxRow; r < boxRow + SUBGRID_SIZE; ++r) {
                    for (int c = boxCol; c < boxCol + SUBGRID_SIZE; ++c) {
                        if ((r != i || c != j) && board[r][c] == val) {
                            return false;
                        }
                    }
                }
            }
        }
    }
    return true;
}

bool SudokuBoard::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not create save file '" << filename << "'\n";
        return false;
    }
    
    file << difficulty << "\n";
    
    for (const auto& row : board) {
        for (int cell : row) {
            file << cell << " ";
        }
        file << "\n";
    }
    
    for (const auto& row : fixed) {
        for (bool isFixed : row) {
            file << (isFixed ? 1 : 0) << " ";
        }
        file << "\n";
    }
    
    file.close();
    return true;
}

bool SudokuBoard::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open save file '" << filename << "'\n";
        return false;
    }
    
    file >> difficulty;
    if (file.fail()) {
        std::cerr << "Error: Corrupted save file (difficulty)\n";
        file.close();
        return false;
    }
    
    for (auto& row : board) {
        for (int& cell : row) {
            file >> cell;
            if (file.fail()) {
                std::cerr << "Error: Corrupted save file (board data)\n";
                file.close();
                return false;
            }
        }
    }
    
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j) {
            int val;
            file >> val;
            if (file.fail()) {
                std::cerr << "Error: Corrupted save file (fixed data)\n";
                file.close();
                return false;
            }
            fixed[i][j] = (val == 1);
        }
    }
    
    file.close();
    return true;
}

int SudokuBoard::getCell(int row, int col) const {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return -1;
    }
    return board[row][col];
}

bool SudokuBoard::isCellFixed(int row, int col) const {
    if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
        return false;
    }
    return fixed[row][col];
}