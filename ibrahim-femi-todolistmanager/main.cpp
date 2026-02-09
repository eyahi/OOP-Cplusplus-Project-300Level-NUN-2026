/**
 * @file main.cpp
 * @author SIAO Development Team (SHOKOYA IBRAHIM AND OLORUNFEMI AKINWALE)
 * @date 2026-01-20
 * @brief Implementation file for Todo List Manager application
 * 
 * This file contains the implementation of TodoListWindow and the main
 * function. It provides a Qt-based GUI for managing a todo list with
 * features for adding, toggling, deleting, and persisting tasks.
 * 
 * Course: C++ Programming (OOP)
 * Assignment: Todo List Manager - GUI Implementation
 */

#include "main.h"
#include <QApplication>
#include <fstream>
#include <string>

/**
 * @brief Constructor implementation
 * 
 * Sets up the GUI layout, creates all widgets, connects signals/slots,
 * loads existing tasks, and initializes the UI state.
 */
TodoListWindow::TodoListWindow(QWidget *parent) 
    : QMainWindow(parent),
      taskInput(nullptr),
      addButton(nullptr),
      taskList(nullptr),
      saveButton(nullptr),
      exitButton(nullptr),
      statusLabel(nullptr),
      tasks(),
      filename(DEFAULT_FILENAME),
      needsSave(false) {
    setWindowTitle("SIAO Todo List");
    setMinimumSize(WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT);
    
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    QLabel *titleLabel = new QLabel("SIAO Todo List", this);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin: 10px;");
    titleLabel->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(titleLabel);
    
    QHBoxLayout *addTaskLayout = new QHBoxLayout(centralWidget);

    taskInput = new QLineEdit(this);
    taskInput->setPlaceholderText("Enter new task...");
    addTaskLayout->addWidget(taskInput);

    addButton = new QPushButton("Add Task", this);
    addTaskLayout->addWidget(addButton);
    mainLayout->addLayout(addTaskLayout);
    
    connect(addButton, &QPushButton::clicked, this, &TodoListWindow::addTask);
    connect(taskInput, &QLineEdit::returnPressed, this, &TodoListWindow::addTask);
    
    QLabel *listLabel = new QLabel("Tasks:", this);
    mainLayout->addWidget(listLabel);

    taskList = new QListWidget(this);
    taskList->setAlternatingRowColors(true);
    mainLayout->addWidget(taskList);
    
    QHBoxLayout *buttonLayout = new QHBoxLayout(centralWidget);

    saveButton = new QPushButton("Save", this);
    saveButton->setStyleSheet("background-color: #4CAF50; color: white; padding: 8px;");
    buttonLayout->addWidget(saveButton);
    connect(saveButton, &QPushButton::clicked, this, &TodoListWindow::saveTasks);

    statusLabel = new QLabel("All changes saved", this);
    statusLabel->setStyleSheet("color: green;");
    buttonLayout->addWidget(statusLabel);

    buttonLayout->addStretch();

    exitButton = new QPushButton("Exit", this);
    exitButton->setStyleSheet("background-color: #f44336; color: white; padding: 8px;");
    buttonLayout->addWidget(exitButton);
    connect(exitButton, &QPushButton::clicked, this, &TodoListWindow::close);
    mainLayout->addLayout(buttonLayout);
    
    loadTasks();
    updateTaskList();
}

/**
 * @brief Destructor implementation
 * 
 * Saves tasks automatically if there are unsaved changes before the
 * window is destroyed. Qt's parent-child system handles widget cleanup.
 */
TodoListWindow::~TodoListWindow() {
    if (needsSave) {
        saveTasks();
    }
}

/**
 * @brief Adds a new task to the list
 * 
 * Validates input (empty check, length limit), creates a new Task object,
 * adds it to the tasks vector, clears the input field, updates the UI,
 * and marks that changes need saving.
 */
void TodoListWindow::addTask() {
    QString text = taskInput->text().trimmed();

    if (text.isEmpty()) {
        return;
    }

    if (text.length() > MAX_TASK_LENGTH) {
        QMessageBox::warning(this, "Input Error", 
            QString("Task description is too long. Maximum length is %1 characters.").arg(MAX_TASK_LENGTH));
        return;
    }

    try {
        Task newTask;
        newTask.description = text.toStdString();
        newTask.isCompleted = false;

        tasks.push_back(newTask);

        taskInput->clear();
        updateTaskList();

        markNeedsSave();
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Error", 
            QString("Failed to add task: %1").arg(e.what()));
    }
}
    
/**
 * @brief Toggles the completion status of a task
 * @param index The index of the task to toggle (0-based)
 * 
 * Validates the index is within bounds, toggles the completion status,
 * refreshes the UI to show the updated state, and marks changes for saving.
 */
bool TodoListWindow::isValidIndex(int index) const {
    return index >= 0 && index < static_cast<int>(tasks.size());
}

void TodoListWindow::toggleTask(int index) {
    if (!isValidIndex(index)) {
        return;
    }

    try {
        tasks[index].isCompleted = !tasks[index].isCompleted;

        updateTaskList();
        markNeedsSave();
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", 
            QString("Failed to toggle task: %1").arg(e.what()));
    }
}

/**
 * @brief Deletes a task from the list
 * @param index The index of the task to delete (0-based)
 * 
 * Validates the index is within bounds, removes the task from the vector,
 * refreshes the UI, and marks changes for saving.
 */
void TodoListWindow::deleteTask(int index) {
    if (!isValidIndex(index)) {
        return;
    }

    try {
        tasks.erase(tasks.begin() + index);

        updateTaskList();
        markNeedsSave();
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Error", 
            QString("Failed to delete task: %1").arg(e.what()));
    }
}
    
/**
 * @brief Saves all tasks to the file
 * 
 * Opens the file for writing, writes each task in the format "description|status",
 * checks for write errors after each write, closes the file, and updates the
 * status indicator. Handles file errors gracefully with user-friendly messages.
 */
void TodoListWindow::saveTasks() {
    if (filename.empty()) {
        QMessageBox::warning(this, "Error", "Invalid filename. Cannot save tasks.");
        return;
    }

    try {
        std::ofstream file(filename);

        if (!file.is_open()) {
            QMessageBox::warning(this, "Save Error", 
                QString("Could not save tasks to file: %1\n\nPlease check file permissions and disk space.").arg(QString::fromStdString(filename)));
            return;
        }

        // Write each task to file
        for (const Task &task : tasks) {
            file << task.description << FILE_SEPARATOR;
            file << (task.isCompleted ? "1" : "0") << std::endl;
            
            if (file.fail()) {
                file.close();
                QMessageBox::warning(this, "Save Error", 
                    QString("Error writing to file: %1").arg(QString::fromStdString(filename)));
                return;
            }
        }

        file.close();

        needsSave = false;
        statusLabel->setText("All changes saved");
        statusLabel->setStyleSheet("color: green;");
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "Save Error", 
            QString("Error saving tasks: %1").arg(e.what()));
    }
}
    
/**
 * @brief Handles window close events
 * @param event The close event to process
 * 
 * If there are unsaved changes, shows a dialog asking the user to save,
 * discard, or cancel. Based on the user's choice, saves tasks, allows
 * close, or prevents closing. If no unsaved changes, allows normal close.
 */
void TodoListWindow::closeEvent(QCloseEvent *event) {
    if (needsSave) {
        QMessageBox::StandardButton reply = QMessageBox::question(
            this,
            "Unsaved Changes",
            "You have unsaved changes. Save before exiting?",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
        );

        if (reply == QMessageBox::Save) {
            saveTasks();
            event->accept();
        } else if (reply == QMessageBox::Discard) {
            needsSave = false;
            event->accept();
        } else {
            event->ignore();
        }
    } else {
        event->accept();
    }
}

/**
 * @brief Loads tasks from the file
 * 
 * Opens the tasks file, reads each line, parses the format "description|status",
 * validates the data (skips invalid lines), adds valid tasks to the vector,
 * and handles errors gracefully. Missing files are normal for first run.
 */
void TodoListWindow::loadTasks() {
    if (filename.empty()) {
        return;
    }

    try {
        std::ifstream file(filename);

        if (!file.is_open()) {
            // File doesn't exist yet - normal for first run
            return;
        }

        std::string line;

        // Read file line by line and parse each task
        while (std::getline(file, line)) {
            // Skip empty lines
            if (line.empty()) {
                continue;
            }
            
            // Find the separator character
            size_t separatorPos = line.find(FILE_SEPARATOR);
            
            // Skip lines without separator or with separator at the start
            if (separatorPos == std::string::npos || separatorPos == 0) {
                continue;
            }

            // Extract description and status
            Task task;
            task.description = line.substr(0, separatorPos);
            
            // Skip if description is too long
            if (task.description.length() > MAX_TASK_LENGTH) {
                continue;
            }

            // Parse status: "1" means completed, anything else means incomplete
            std::string statusStr = line.substr(separatorPos + 1);
            task.isCompleted = (statusStr == "1");

            tasks.push_back(task);
        }

        file.close();
    } catch (const std::exception &e) {
        QMessageBox::warning(this, "Load Error", 
            QString("Error loading tasks: %1").arg(e.what()));
    }
}
    
/**
 * @brief Updates the task list display in the UI
 * 
 * Clears the current list widget, creates new widget items for each task
 * by calling createTaskItem, and displays an empty message if no tasks exist.
 * This is called whenever the task list changes (add, delete, toggle).
 */
void TodoListWindow::updateTaskList() {
    taskList->clear();

    // Create a widget item for each task
    for (size_t i = 0; i < tasks.size(); ++i) {
        createTaskItem(static_cast<int>(i));
    }

    // Show empty message if no tasks exist
    if (tasks.empty()) {
        QLabel *emptyLabel = new QLabel("No tasks yet. Add one above!", this);
        emptyLabel->setAlignment(Qt::AlignCenter);
        emptyLabel->setStyleSheet("color: gray; padding: 20px;");

        QListWidgetItem *emptyItem = new QListWidgetItem(taskList);
        emptyItem->setSizeHint(emptyLabel->sizeHint());
        taskList->setItemWidget(emptyItem, emptyLabel);
    }
}

/**
 * @brief Creates a widget item for displaying a single task
 * @param index The index of the task to display (0-based)
 * 
 * Creates a custom widget containing a checkbox (for completion status),
 * a label (for description with styling based on completion), and a delete
 * button. Connects signals for user interactions and adds the widget to the list.
 */
void TodoListWindow::createTaskItem(int index) {
    QWidget *itemWidget = new QWidget(taskList);
    QHBoxLayout *itemLayout = new QHBoxLayout(itemWidget);
    itemLayout->setContentsMargins(WIDGET_MARGIN, WIDGET_MARGIN, WIDGET_MARGIN, WIDGET_MARGIN);

    QCheckBox *checkbox = new QCheckBox(itemWidget);
    checkbox->setChecked(tasks[index].isCompleted);
    connect(checkbox, &QCheckBox::clicked, [this, index]() { toggleTask(index); });
    itemLayout->addWidget(checkbox);

    QLabel *descLabel = new QLabel(QString::fromStdString(tasks[index].description), itemWidget);
    if (tasks[index].isCompleted) {
        descLabel->setStyleSheet("color: gray; text-decoration: line-through;");
    }
    itemLayout->addWidget(descLabel, 1);

    QPushButton *deleteBtn = new QPushButton("Delete", itemWidget);
    deleteBtn->setStyleSheet("background-color: #f44336; color: white; padding: 4px 8px;");
    connect(deleteBtn, &QPushButton::clicked, [this, index]() { deleteTask(index); });
    itemLayout->addWidget(deleteBtn);

    QListWidgetItem *listItem = new QListWidgetItem(taskList);
    listItem->setSizeHint(itemWidget->sizeHint());
    taskList->setItemWidget(listItem, itemWidget);
}
    
/**
 * @brief Marks that there are unsaved changes
 * 
 * Sets the needsSave flag to true and updates the status label to indicate
 * that changes are unsaved (orange color). This is called whenever the task
 * list is modified (add, delete, toggle).
 */
void TodoListWindow::markNeedsSave() {
    needsSave = true;
    statusLabel->setText("Changes unsaved");
    statusLabel->setStyleSheet("color: orange;");
}

/**
 * @brief Main function - application entry point
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return Exit code (0 for success)
 * 
 * Creates the Qt application, instantiates the main window, shows it,
 * and starts the event loop. The application runs until the window is closed.
 */
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    TodoListWindow window;

    window.show();

    return app.exec();
}
