Qt Sudoku 🎲
Project Overview

Qt Sudoku is a desktop Sudoku puzzle game developed in C++ using Qt 6 (Widgets) for the graphical user interface.

The application provides a clean, modern interface for playing classic 9×9 Sudoku puzzles. It includes puzzle generation, real-time validation, hints, timer, difficulty levels, and the ability to save/load games.

This project was built to demonstrate:

Object-oriented programming in C++
GUI development with Qt 6 (Widgets)
2D grid manipulation and logic implementation
Event handling and user interaction
File I/O for saving/loading game states
Input validation and error prevention

Features

🧩 Multiple difficulty levels (Easy, Medium, Hard, Expert)
🔢 Classic 9×9 Sudoku grid with number input
✅ Real-time conflict detection (row, column, 3×3 block)
💡 Hint system (reveals one correct number)
⏱ Game timer with pause/resume
💾 Save and load game progress
🔄 New game generation with multiple puzzle patterns
🖱 Mouse and keyboard input support
🎨 Clean, responsive, and modern-looking interface
⚠️ Prevents invalid moves with visual feedback
🏆 Completion celebration with congratulations dialog

Technologies Used

Language:     C++ (C++17)
GUI Framework: Qt 6 (Widgets)
Build System:  CMake
Platform:      Windows (cross-platform compatible with minor adjustments)

Project Structure
QtSudoku/
│
├── main.cpp
├── mainwindow.h
├── mainwindow.cpp
├── sudokugrid.h
├── sudokugrid.cpp
├── sudokugenerator.h
├── sudokugenerator.cpp
├── CMakeLists.txt
├── README.txt
└── resources/           (icons, stylesheets if used)

⚠️ Note: UI is built programmatically in C++ (no .ui files from Qt Designer are used).

Build Instructions (Qt Creator – Recommended)

Requirements

Qt 6.x (Widgets module)
CMake ≥ 3.16
MinGW or compatible C++ compiler

Steps

1. Open Qt Creator
2. Select "Open Project"
3. Choose CMakeLists.txt
4. Configure the project with a Qt 6 Kit
5. Build and Run

How to Play

Launch the application
Click "New Game" and select difficulty
Click on any empty cell and type a number (1–9) or use the on-screen number pad
Green = correct placement
Red = conflict (same row/column/block)
Use Hint button when stuck
Save progress with File → Save Game
Load previous game with File → Load Game

Error Handling

The application gracefully handles:
Invalid number input
Loading corrupted/incompatible save files
Attempting to place numbers in pre-filled cells
User-friendly warning messages are shown via QMessageBox

Known Limitations

No pencil marks / candidate notes (classic mode only)
No auto-solver (only human play + hints)
No online puzzle downloading
No multiple undo/redo stack (single-step undo only)

Authors

Ajoku Edmonson 20230789
Benjamin Uchechukwu 20233482
Ibom Chinonso 20231681
C++ / Qt Sudoku Project

License

This project is for educational purposes.

