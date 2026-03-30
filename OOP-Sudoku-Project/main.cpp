// main.cpp
// Sudoku Game Project
// A complete Sudoku implementation with save/load functionality
// Compilation: g++ -std=c++17 -Wall -Wextra -o sudoku main.cpp SudokuBoard.cpp GameManager.cpp

#include "GameManager.h"
#include <iostream>
#include <limits>
#include <cstdlib>

const std::string MARGIN = "                              ";

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void setupWindow() {
    #ifdef _WIN32
        system("mode con: cols=100 lines=30");
        system("title Sudoku Master v2.0");
    #endif
}

int getMenuChoice() {
    int choice;
    clearScreen();
    std::cout << "\n\n\n" << MARGIN << "\033[1;33m========= MAIN MENU =========\033[0m\n";
    std::cout << MARGIN << "\033[1;36m [1] Play New Random Game\033[0m\n";
    std::cout << MARGIN << "\033[1;36m [2] Load Saved Game\033[0m\n";
    std::cout << MARGIN << "\033[1;36m [3] Delete Saved Game\033[0m\n";
    std::cout << MARGIN << "\033[1;36m [4] Exit\033[0m\n";
    std::cout << MARGIN << "\033[1;33m=============================\033[0m\n";
    std::cout << MARGIN << "Selection: ";
    
    std::cin >> choice;
    if (std::cin.fail()) {
        clearInputBuffer();
        return -1;
    }
    clearInputBuffer();
    return choice;
}

int main() {
    setupWindow();
    GameManager manager;
    bool running = true;
    
    // Centered Splash Screen
    clearScreen();
    std::cout << "\n\n\n\n\n" << MARGIN << "\033[1;36m*******************************\n";
    std::cout << MARGIN << "*   SUDOKU MASTER v2.0   *\n";
    std::cout << MARGIN << "*******************************\033[0m\n";
    std::cout << "\n" << MARGIN << "    [ Press Enter to Start ]";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    while (running) {
        int choice = getMenuChoice();
        
        switch (choice) {
            case 1:
                manager.playNewGame();
                break;
            case 2:
                manager.loadGame();
                break;
            case 3:
                manager.deleteSave();
                break;
            case 4:
                clearScreen();
                std::cout << "\n\n\n" << MARGIN << "\033[1;36mThanks for playing!\033[0m\n\n";
                running = false;
                break;
            default:
                std::cout << "\n" << MARGIN << "\033[1;31mInvalid choice. Please enter 1-4.\033[0m\n";
                std::cout << MARGIN << "Press Enter to continue...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
        }
    }
    
    return 0;
}