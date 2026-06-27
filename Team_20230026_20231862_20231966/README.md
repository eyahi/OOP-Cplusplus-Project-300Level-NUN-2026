
# Sudoku GUI

**C++ | Win32 API | Object-Oriented Programming**

A native Windows Sudoku application developed in **C++** using **OOP principles** and the **Win32 API** (no external GUI frameworks).

---

## 👥 Group Members

1. Amos Agor
2. Gideon Ansa
3. Bright fomah

---

## 📌 Project Overview

This application implements a fully functional **9×9 Sudoku game** with a graphical user interface.
The program follows Object-Oriented Programming concepts such as **encapsulation, modular design, and separation of concerns**.

---

## ✨ Features

* 9×9 editable Sudoku grid
* Input digits **1–9** (Backspace to clear)
* Pre-filled clue cells are **read-only**
* Automatic conflict detection (row, column, and 3×3 box)
* Functional buttons:

  * **New (Sample)** – Load a sample puzzle
  * **Clear User** – Clear only user inputs
  * **Clear All** – Reset entire board
  * **Check** – Validate current solution
  * **Solve** – Auto-solve using backtracking algorithm

---

## 🛠️ Build Instructions (Windows – MSVC)

### Option 1: Using CMake (Command Line)

From the project folder:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release
.\build\Release\SudokuGUI.exe
```

### Option 2: Using VS Code (CMake Tools Extension)

1. Open the project folder in VS Code
2. Select kit: **MSVC x64**
3. Build and run the `SudokuGUI` target

---

## 🧩 Code Structure (OOP Design)

### `include/Sudoku/`

* **Cell** – Represents a single Sudoku cell
* **Board** – Handles validation and conflict detection
* **Solver** – Implements backtracking algorithm

### `include/App/`

* **Controller** – Manages application logic
* **GridView** – Displays the 9×9 Sudoku grid (Win32 view)
* **MainWindow** – Main window, layout, and buttons

---


