# GROUP MEMBERS
- Tomvie Bryan Lott  20230027
- Daniel Etta Ononse 20230040
- Favour Arhyel Bwala 20231586

# Sudoku Game

A complete object-oriented Sudoku game implementation in C++ with save/load functionality.

## Project Information

**Course:** Object-Oriented Programming  
**Assignment:** C++ Project  
**Date:** January 20, 2026

## Description

This is a fully functional Sudoku game that allows players to:
- Generate random Sudoku boards with three difficulty levels
- Play the game with input validation
- Save and load game progress
- Delete saved games

The project demonstrates proper OOP principles, memory management, error handling, and modern C++ best practices.

## Files Included

- `main.cpp` - Program entry point and main menu
- `SudokuBoard.h` - SudokuBoard class header
- `SudokuBoard.cpp` - SudokuBoard class implementation
- `GameManager.h` - GameManager class header
- `GameManager.cpp` - GameManager class implementation
- `README.md` - This file

## Compilation Instructions

### Linux/Mac:
```bash
g++ -std=c++17 -Wall -Wextra -o sudoku main.cpp SudokuBoard.cpp GameManager.cpp
```

### Windows (MinGW):
```bash
g++ -std=c++17 -Wall -Wextra -o sudoku.exe main.cpp SudokuBoard.cpp GameManager.cpp
```

### With memory debugging (Linux/Mac):
```bash
g++ -std=c++17 -Wall -Wextra -fsanitize=address -g -o sudoku main.cpp SudokuBoard.cpp GameManager.cpp
```

## Execution Instructions

### Linux/Mac:
```bash
./sudoku
```

### Windows:
```bash
sudoku.exe
```

## Features

1. **Random Board Generation**
   - Three difficulty levels (Easy, Medium, Hard)
   - Guaranteed solvable boards
   - Random number placement for variety

2. **Gameplay**
   - Interactive board display
   - Move validation (no conflicts in rows, columns, or 3x3 boxes)
   - Cannot modify fixed cells
   - Clear cells by entering 0
   - Automatic win detection

3. **Save/Load System**
   - Save game progress at any time
   - Load previously saved games
   - Resume exactly where you left off

4. **Delete Saved Games**
   - Remove saved game files
   - Confirmation prompt to prevent accidents

## How to Play

1. **Start the program** - Run the compiled executable
2. **Choose from the main menu:**
   - `1` - Start a new random game
   - `2` - Load a saved game
   - `3` - Delete saved game
   - `4` - Exit

3. **During gameplay:**
   - The board displays with rows (A-I) and columns (1-9)
   - `.` represents empty cells
   - **Green numbers** are fixed cells (original puzzle - cannot be changed)
   - **White numbers** are your entries (can be modified)

4. **Making moves:**
   - Enter: `set A3 5` to place 5 in row A, column 3
   - Enter: `set E7 9` to place 9 in row E, column 7
   - Type: `save` to save your current progress
   - Type: `exit` to quit (with option to save)

5. **Win condition:**
   - Fill all cells correctly (no conflicts)
   - Game automatically detects when you've won

## Sample Usage

```
Welcome to Sudoku!

*******************************
*   SUDOKU MASTER v2.0   *
*******************************

    [ Press Enter to Start ]

========= MAIN MENU =========
 [1] Play New Random Game
 [2] Load Saved Game
 [3] Delete Saved Game
 [4] Exit
=============================
Selection: 1

==== SELECT DIFFICULTY ====
 [1] Easy (30 empty cells)
 [2] Medium (40 empty cells)
 [3] Hard (50 empty cells)
===========================
Selection: 2

Generating new Sudoku board...
Board generated! Good luck!

    1 2 3   4 5 6   7 8 9
A   5 3 . | . 7 . | . . .
B   6 . . | 1 9 5 | . . .
C   . 9 8 | . . . | . 6 .
    ------+-------+------
D   8 . . | . 6 . | . . 3
E   4 . . | 8 . 3 | . . 1
F   7 . . | . 2 . | . . 6
    ------+-------+------
G   . 6 . | . . . | 2 8 .
H   . . . | 4 1 9 | . . 5
I   . . . | 3 8 6 | . 7 9

Cmd: 'set A3 9' | 'save' | 'exit'
Input: set A3 4
```

## Input File Requirements

No external input files are required. The game generates random boards internally and creates save files as needed.

**Save file:** `sudoku_save.txt` (automatically created when saving)

## Known Issues and Limitations

- Board generation may take a moment for harder difficulties
- Only one save slot available (new saves overwrite old ones)
- No undo/redo functionality
- No hint system

## Technical Highlights

### OOP Design
- Separation of concerns (Board logic vs. Game management)
- Encapsulation of board state
- Clean class interfaces

### Memory Management
- Smart pointers (`std::unique_ptr`) used to prevent memory leaks
- RAII principles followed
- No raw `new`/`delete` operations
- STL containers manage their own memory

### Error Handling
- Input validation on all user inputs
- File operation error checking
- Graceful recovery from invalid moves
- Clear error messages for users

### Modern C++ Features
- STL containers (`std::vector`, `std::string`)
- Smart pointers for automatic memory management
- Range-based for loops
- Const correctness throughout
- `nullptr` instead of NULL

### Code Quality
- Comprehensive comments and documentation
- Consistent naming conventions
- Proper const usage
- No magic numbers (constants defined)
- Modular design with single responsibility principle

## Dependencies

- C++17 standard library
- No external libraries required
