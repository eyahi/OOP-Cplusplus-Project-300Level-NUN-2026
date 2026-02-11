# ============================================================
#  Velame Manager – Quality Assured
#  Qt C++ To-Do List Application
# ============================================================

# ------------------------------------------------------------
#  GROUP MEMBERS
# ------------------------------------------------------------
# Chidi Chiemezie Abdiel   - 20232119
# Victor Olasehinde       - 20230660
# Fola Ogunsola           - 20230056

# ------------------------------------------------------------
#  PROJECT DESCRIPTION
# ------------------------------------------------------------
# Velame Manager is a desktop To-Do List application built using
# C++ and the Qt 6 Widgets framework.
#
# The application allows users to:
# - Add tasks
# - Edit tasks
# - Delete tasks
# - Mark tasks as completed
# - Sort tasks by due date
#
# The project demonstrates GUI development, signal-slot
# communication, and table-based data handling using Qt.

# ------------------------------------------------------------
#  TECHNOLOGIES USED
# ------------------------------------------------------------
# Programming Language : C++ (C++17)
# Framework            : Qt 6 (Widgets)
# Build System         : CMake
# UI Type              : Desktop GUI (Qt Widgets)

# ------------------------------------------------------------
#  PROJECT FILE STRUCTURE
# ------------------------------------------------------------
# ToDoList/
# ├── CMakeLists.txt        -> Build configuration file
# ├── main.cpp              -> Application entry point
# ├── mainwindow.h          -> Main window class declaration
# ├── mainwindow.cpp        -> Main window implementation
# └── README.md             -> Project documentation

# ------------------------------------------------------------
#  HOW THE APPLICATION WORKS
# ------------------------------------------------------------
# 1. The program starts in main.cpp.
# 2. QApplication is created and the Fusion style is applied.
# 3. The MainWindow object is created and displayed.
# 4. The user interacts with the GUI to manage tasks.

# ------------------------------------------------------------
#  MAIN WINDOW COMPONENTS
# ------------------------------------------------------------
# Sidebar:
# - Task title input (QLineEdit)
# - Task description input (QTextEdit)
# - Due date selector (QDateEdit)
# - Status selector (QComboBox)
# - Priority selector (QComboBox)
# - Add Task button (QPushButton)
#
# Main Area:
# - Task table (QTableWidget)
# - Buttons for:
#   * Complete task
#   * Edit task
#   * Delete task
#   * Sort by date

# ------------------------------------------------------------
#  TASK MANAGEMENT LOGIC
# ------------------------------------------------------------
# addTask():
# - Adds a new task to the table using user input
#
# deleteTask():
# - Removes the currently selected task
#
# markCompleted():
# - Marks a selected task as completed
# - Updates the row color
#
# editTask():
# - Loads task data back into input fields
# - Deletes the old task for re-entry
#
# sortByDate():
# - Sorts tasks using the due date column

# ------------------------------------------------------------
#  VISUAL STYLING
# ------------------------------------------------------------
# - Dark themed interface using Qt Style Sheets
# - Completed tasks appear green
# - Overdue tasks appear red
# - Priority levels affect row color
#
# This improves readability and task tracking.

# ------------------------------------------------------------
#  BUILD AND RUN INSTRUCTIONS
# ------------------------------------------------------------
# Requirements:
# - Qt 6 installed
# - CMake version 3.16 or higher
# - C++17 compatible compiler
#
# Build Steps:
# mkdir build
# cd build
# cmake ..
# cmake --build .
#
# Run the generated executable after build.

# ------------------------------------------------------------
#  PURPOSE OF THE PROJECT
# ------------------------------------------------------------
# This project is suitable for:
# - Academic submission
# - Learning Qt GUI programming
# - Demonstrating C++ OOP concepts
# - Understanding signal and slot connections
#
# ============================================================
