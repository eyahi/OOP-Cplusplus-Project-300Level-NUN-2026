/**
 * @file main.h
 * @author SIAO Development Team (SHOKOYA IBRAHIM AND OLORUNFEMI AKINWALE)
 * @date 2026-01-20
 * @brief Header file for Todo List Manager application
 * 
 * This file contains the class definition for TodoListWindow, a Qt-based
 * GUI application for managing a todo list. The application allows users to
 * add, toggle, delete, and save tasks with persistent storage.
 * 
 * Course: C++ Programming (OOP)
 * Assignment: Todo List Manager - GUI Implementation
 */

//  Prevents multiple inclusion and Ensures the file is only included once during compilation
#ifndef MAIN_H
#define MAIN_H

// These are QT GUI and Cores
#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include <QCloseEvent>
#include <QFile>
#include <QString>
#include <QTextStream>

#include <string>
#include <vector>

/**
 * @struct Task
 * @brief Represents a single todo list task
 * 
 * Stores the description and completion status of a task.
 * Tasks are persisted to disk in the format: "description|status"
 * where status is "0" for incomplete and "1" for completed.
 */
struct Task {
  std::string description;  ///< The task description text
  bool isCompleted;         ///< Whether the task is completed (true) or not (false)
};

/**
 * @class TodoListWindow
 * @brief Main window class for the Todo List Manager application
 * 
 * This class extends QMainWindow to provide a GUI interface for managing
 * a todo list. It handles task creation, modification, deletion, and
 * persistent storage using Qt's widget system and file I/O.
 * 
 * Features:
 * - Add new tasks via text input
 * - Toggle task completion status
 * - Delete tasks
 * - Save/load tasks from file
 * - Unsaved changes tracking
 * - Error handling
 */

class TodoListWindow : public QMainWindow {
  Q_OBJECT // This is a macro that allows the class to be used as a Qt object

public:
  /**
   * @brief Constructor for TodoListWindow
   * @param parent Parent widget (nullptr for top-level window)
   * 
   * Initializes the GUI, sets up widgets and layouts, connects signals/slots,
   * and loads existing tasks from file.
   */
  explicit TodoListWindow(QWidget *parent = nullptr);

  /**
   * @brief Destructor for TodoListWindow
   * 
   * Saves tasks if there are unsaved changes before destroying the window.
   */
  ~TodoListWindow();

  // Prevent copying (QObject-derived classes should not be copied)
  TodoListWindow(const TodoListWindow &) = delete;
  TodoListWindow &operator=(const TodoListWindow &) = delete;

private slots:
  /**
   * @brief Adds a new task to the list
   * 
   * Reads text from the input field, validates it, creates a new Task,
   * adds it to the tasks vector, and updates the UI. Also marks that
   * changes need to be saved.
   * 
   * Preconditions: taskInput widget must be initialized
   * Side effects: Updates tasks vector, clears input field, refreshes UI
   */
  void addTask();

  /**
   * @brief Toggles the completion status of a task
   * @param index The index of the task to toggle (0-based)
   * 
   * Validates the index, toggles the task's completion status,
   * updates the UI, and marks that changes need to be saved.
   * 
   * Preconditions: index must be valid (0 <= index < tasks.size())
   * Side effects: Modifies tasks[index].isCompleted, updates UI
   */
  void toggleTask(int index);

  /**
   * @brief Deletes a task from the list
   * @param index The index of the task to delete (0-based)
   * 
   * Validates the index, removes the task from the tasks vector,
   * updates the UI, and marks that changes need to be saved.
   * 
   * Preconditions: index must be valid (0 <= index < tasks.size())
   * Side effects: Removes task from tasks vector, updates UI
   */
  void deleteTask(int index);

  /**
   * @brief Saves all tasks to the file
   * 
   * Opens the file for writing, writes all tasks in the format
   * "description|status", handles errors gracefully, and updates
   * the status label to indicate successful save.
   * 
   * Preconditions: filename must be set and valid
   * Side effects: Writes to file, updates needsSave flag and status label
   * @return void (errors are shown via QMessageBox)
   */
  void saveTasks();

  /**
   * @brief Handles window close events
   * @param event The close event to process
   * 
   * If there are unsaved changes, prompts the user to save, discard,
   * or cancel. Otherwise, allows the window to close normally.
   * 
   * Preconditions: None
   * Side effects: May save tasks, may prevent window from closing
   */
  void closeEvent(QCloseEvent *event) override;

private:
  /**
   * @brief Validates that an index is within the valid range for tasks
   * @param index The index to validate
   * @return true if index is valid, false otherwise
   */
  bool isValidIndex(int index) const;

  /**
   * @brief Loads tasks from the file
   * 
   * Opens the tasks file, reads each line, parses task data,
   * validates format, and adds valid tasks to the tasks vector.
   * Handles missing files gracefully (normal for first run).
   * 
   * Preconditions: filename must be set
   * Side effects: Populates tasks vector, may show warning messages
   */
  void loadTasks();

  /**
   * @brief Updates the task list display in the UI
   * 
   * Clears the current list widget, creates new widget items for
   * each task, and displays them. Shows an empty message if no tasks exist.
   * 
   * Preconditions: taskList widget must be initialized
   * Side effects: Rebuilds entire task list display
   */
  void updateTaskList();
  
  /**
   * @brief Creates a widget item for displaying a single task
   * @param index The index of the task to display (0-based)
   * 
   * Creates a custom widget containing a checkbox, label, and delete
   * button for the specified task. Applies styling based on completion
   * status and connects signals for user interactions.
   * 
   * Preconditions: index must be valid (0 <= index < tasks.size())
   * Side effects: Creates new widgets and adds them to taskList
   */
  void createTaskItem(int index);

  /**
   * @brief Marks that there are unsaved changes
   * 
   * Sets the needsSave flag to true and updates the status label
   * to indicate that changes are unsaved.
   * 
   * Preconditions: statusLabel must be initialized
   * Side effects: Updates needsSave flag and status label text/style
   */
  void markNeedsSave();

  // GUI Widgets
  QLineEdit *taskInput;      ///< Text input field for adding new tasks
  QPushButton *addButton;     ///< Button to add a new task
  QListWidget *taskList;      ///< List widget displaying all tasks
  QPushButton *saveButton;    ///< Button to manually save tasks
  QPushButton *exitButton;    ///< Button to exit the application
  QLabel *statusLabel;        ///< Label showing save status (saved/unsaved)

  // Data members
  std::vector<Task> tasks;    ///< Vector storing all tasks
  std::string filename;       ///< Filename for task storage (default: "tasks.txt")
  bool needsSave;             ///< Flag indicating if there are unsaved changes
  
  // Constants
  static constexpr const char* DEFAULT_FILENAME = "tasks.txt";  ///< Default filename for task storage
  static constexpr int WINDOW_MIN_WIDTH = 600;                  ///< Minimum window width in pixels
  static constexpr int WINDOW_MIN_HEIGHT = 500;                 ///< Minimum window height in pixels
  static constexpr int MAX_TASK_LENGTH = 1000;                 ///< Maximum task description length
  static constexpr int WIDGET_MARGIN = 5;                       ///< Margin for widget layouts
  static constexpr char FILE_SEPARATOR = '|';                   ///< Separator character in task file
  static constexpr char STATUS_COMPLETE = '1';                  ///< Character representing completed task
  static constexpr char STATUS_INCOMPLETE = '0';               ///< Character representing incomplete task
};

#endif
