# Todo List Manager

A modern GUI-based todo list application written in C++ using Qt Widgets.

## Author

**SIAO Development Team**
PARTICIPANTS
1. SHOKOYA IBRAHIM 246240011 (ICE)
2. AKINWALE OLORUNFEMI 246240015 (ICE)

## Project Description

This application provides a user-friendly graphical interface for managing a todo list. Users can add, complete, and delete tasks with persistent storage to a text file. The application features a clean, intuitive interface built with Qt Widgets and includes comprehensive error handling and input validation.

## Features

- ✅ **Add Tasks** - Add new tasks using the input field (press Enter or click Add)
- 📋 **View Tasks** - See all tasks in a scrollable list with alternating row colors
- ✓ **Mark Complete** - Click checkboxes to mark tasks as completed/incomplete
- 🗑️ **Delete Tasks** - Remove tasks with the delete button
- 💾 **Persistent Storage** - Tasks are automatically saved to `tasks.txt`
- 🎨 **Modern GUI** - Beautiful, intuitive Qt-based interface
- 💾 **Auto-save Tracking** - Manual save button and prompts to save on exit
- ⚠️ **Error Handling** - Comprehensive error handling with user-friendly messages
- ✅ **Input Validation** - Validates all user inputs (length, empty checks, etc.)

## Build and Run Instructions

### Prerequisites

- C++ compiler (g++ or clang++ recommended, C++17 or later)
- Qt 5 or Qt 6 (with Qt Widgets module)
- CMake 3.16+ (recommended)

**Installation (macOS with Homebrew):**
```bash
brew install qt cmake
```

**Installation (Linux - Ubuntu/Debian):**
```bash
sudo apt-get install qt6-base-dev qt6-base-dev-tools cmake build-essential
```

### Quick Start

**Step 1: Build the Application**

Open Terminal in the project directory and run:

```bash
mkdir build
cd build
cmake ..
make
```

**Step 2: Run the Application**

**On macOS (easiest):**
```bash
open TodoListManager.app
```

**On macOS (command line):**
```bash
./TodoListManager.app/Contents/MacOS/TodoListManager
```

**On Linux:**
```bash
./TodoListManager
```

**On Windows:**
```bash
build\Debug\TodoListManager.exe
```

### All-in-One Command

You can build and run in one go (macOS):

```bash
mkdir -p build && cd build && cmake .. && make && open TodoListManager.app
```

### Alternative Build Methods

#### Using qmake (Alternative to CMake)

```bash
qmake -project "QT += widgets"
qmake
make
```

#### Manual Compilation (if CMake/qmake not available)

```bash
# Set Qt paths (adjust to your Qt installation)
export QTDIR=/path/to/qt
export PATH=$QTDIR/bin:$PATH

# Compile with MOC (Meta-Object Compiler)
moc main.h -o moc_main.cpp
g++ -std=c++17 -fPIC -I$QTDIR/include -I$QTDIR/include/QtCore -I$QTDIR/include/QtGui -I$QTDIR/include/QtWidgets \
    -c main.cpp moc_main.cpp
g++ -std=c++17 -o TodoListManager main.o moc_main.o \
    -L$QTDIR/lib -lQt6Widgets -lQt6Gui -lQt6Core
```

**Note:** The exact library names may vary (Qt5Widgets vs Qt6Widgets). Adjust based on your Qt version.

### What Gets Created

After building, you'll find:
- `build/TodoListManager.app` - The application bundle (macOS) or executable (Linux/Windows)
- `build/tasks.txt` - Task storage file (created automatically when you first save tasks)

## Input File Requirements

### Task File Format

Tasks are stored in `tasks.txt` (created automatically in the application's working directory). The file format is:

```
Task description|0
Another task|1
Completed task|1
```

**Format Rules:**
- One task per line
- Format: `description|status`
- `description`: Task text (max 1000 characters)
- `status`: `0` for incomplete, `1` for completed
- The pipe character (`|`) is used as a separator
- Empty lines are ignored
- Lines without the separator are skipped

**Note:** If a task description contains the pipe character (`|`), it will be split at that character when saved/loaded. This is a known limitation.

### File Location

The `tasks.txt` file is created in the current working directory when you first save tasks. The file is automatically loaded when the application starts.

## Sample Usage

### Example Run

1. **Launch the application:**
   ```bash
   ./build/TodoListManager.app/Contents/MacOS/TodoListManager
   ```

2. **Add a task:**
   - Type "Buy groceries" in the input field
   - Press Enter or click "Add Task"
   - The task appears in the list

3. **Mark task as complete:**
   - Click the checkbox next to "Buy groceries"
   - The task text turns gray with strikethrough
   - Status changes to "Changes unsaved" (orange)

4. **Save tasks:**
   - Click the "Save" button
   - Status changes to "All changes saved" (green)
   - Tasks are written to `tasks.txt`

5. **Add more tasks:**
   - Add "Finish homework" and "Call mom"
   - Both appear in the list

6. **Delete a task:**
   - Click the red "Delete" button next to "Call mom"
   - The task is removed from the list

7. **Exit:**
   - Click "Exit"
   - If there are unsaved changes, you'll be prompted to save, discard, or cancel

### Sample tasks.txt File

After using the application, `tasks.txt` might look like:

```
Buy groceries|1
Finish homework|0
```

## Known Issues and Limitations

1. **Pipe Character in Task Descriptions:**
   - Tasks containing the pipe character (`|`) will be split when saved/loaded
   - Workaround: Avoid using the pipe character in task descriptions

2. **File Path Limitations:**
   - Tasks are saved to `tasks.txt` in the current working directory
   - The filename cannot be changed through the GUI
   - Workaround: Move or rename the file manually if needed

3. **Maximum Task Length:**
   - Task descriptions are limited to 1000 characters
   - Longer descriptions will show an error message
   - This prevents extremely long inputs that could cause UI issues

4. **No Task Editing:**
   - Tasks cannot be edited in-place
   - Workaround: Delete the task and create a new one with the corrected text

5. **Single File Storage:**
   - All tasks are stored in a single file
   - No support for multiple todo lists or categories

6. **No Undo/Redo:**
   - Deleted tasks cannot be recovered
   - Workaround: Manually edit `tasks.txt` if needed

7. **Platform-Specific:**
   - Requires Qt framework installation
   - May need additional setup on some Linux distributions

## Dependencies

### Required Libraries and Tools

1. **Qt Framework** (Qt 5 or Qt 6)
   - Qt Core
   - Qt GUI
   - Qt Widgets
   
   **Installation:**
   - **macOS (Homebrew):** `brew install qt`
   - **Linux (Ubuntu/Debian):** `sudo apt-get install qt6-base-dev qt6-base-dev-tools`
   - **Windows:** Download from [qt.io](https://www.qt.io/download)

2. **CMake** (version 3.16 or later)
   - **macOS:** `brew install cmake`
   - **Linux:** `sudo apt-get install cmake`
   - **Windows:** Download from [cmake.org](https://cmake.org/download/)

3. **C++ Compiler** (C++17 or later)
   - **macOS:** Xcode Command Line Tools (`xcode-select --install`)
   - **Linux:** `sudo apt-get install build-essential`
   - **Windows:** Visual Studio or MinGW

### Optional Tools

- **qmake** (alternative to CMake, comes with Qt)
- **Make** (for convenience Makefile)

### System Requirements

- **Operating System:** macOS, Linux, or Windows
- **Memory:** Minimal (application is lightweight)
- **Disk Space:** ~50MB (including Qt libraries)
- **Display:** GUI application requires a display/X server

## File Structure

```
todo-list-manager/
├── main.cpp           # Main application source code with Qt GUI
├── main.h             # Header file with class definitions
├── CMakeLists.txt     # CMake build configuration
├── Makefile           # Convenience Makefile
├── tasks.txt          # Task storage file (auto-generated)
├── build/             # Build directory (created during build)
└── README.md          # This file
```

## Troubleshooting

### Common Issues

**"Qt not found" or CMake can't find Qt:**
- Ensure Qt is installed: `brew list qt` (macOS) or `dpkg -l | grep qt` (Linux)
- Set `CMAKE_PREFIX_PATH` if Qt is in a non-standard location:
  ```bash
  cmake -DCMAKE_PREFIX_PATH=/path/to/qt ..
  ```
- On macOS with Homebrew, Qt might be at `/opt/homebrew/opt/qt` or `/usr/local/opt/qt`

**"qmake: command not found":**
- Add Qt's bin directory to PATH: `export PATH="/opt/homebrew/opt/qt/bin:$PATH"`
- Or use CMake instead (recommended)

**Linker errors:**
- Ensure Qt libraries are properly installed
- Check that the correct Qt version is being used (Qt5 vs Qt6)
- Verify library paths in CMake configuration

**Runtime errors on macOS:**
- If the app doesn't launch, set library paths:
  ```bash
  export DYLD_FRAMEWORK_PATH=/opt/homebrew/opt/qt/lib
  ```

**Application crashes on startup:**
- Check that Qt libraries are accessible
- Verify file permissions for the executable
- Check console output for error messages

## Technical Details

### Architecture

The application uses **Qt Widgets**, which provides:
- Native-looking GUI components for each platform
- Event-driven programming model
- Automatic memory management through Qt's parent-child system

### Implementation Highlights

- **Error Handling:** Comprehensive try-catch blocks for file operations
- **Input Validation:** Length checks, empty validation, bounds checking
- **Memory Management:** Qt's parent-child relationships for automatic cleanup
- **File I/O:** Robust file reading/writing with error checking
- **User Experience:** Status indicators, save prompts, error messages

## License

This project is part of a C++ programming course assignment.

## Contact

For questions or issues, please refer to the course instructor or development team.
