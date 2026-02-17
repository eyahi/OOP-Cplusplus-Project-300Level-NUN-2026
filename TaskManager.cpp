
#include "TaskStorage.cpp"
#include <algorithm>

class TodoManager {
private:
    vector<Task> tasks;
    FileHandler fileHandler;
    int nextId;

public:
    TodoManager() : fileHandler("tasks.txt") {
        tasks = fileHandler.loadTasks();
        nextId = tasks.empty() ? 1 : tasks.back().getId() + 1;
    }
    
    void addTask(const string& description, const string& date) {
        if (!description.empty()) {
            tasks.push_back(Task(nextId++, description, date, false));
            fileHandler.saveTasks(tasks);
        }
    }
    
    void editTask(int id, const string& description, const string& date) {
        for (auto& task : tasks) {
            if (task.getId() == id) {
                task.setDescription(description);
                task.setDueDate(date);
                fileHandler.saveTasks(tasks);
                return;
            }
        }
    }
    
    const vector<Task>& getAllTasks() const {
        return tasks;
    }
    
    void markComplete(int id) {
        for (auto& task : tasks) {
            if (task.getId() == id) {
                task.setCompleted(true);
                fileHandler.saveTasks(tasks);
                return;
            }
        }
    }
    
    void deleteTask(int id) {
        auto it = remove_if(tasks.begin(), tasks.end(),
                           [id](const Task& t) { return t.getId() == id; });
        
        if (it != tasks.end()) {
            tasks.erase(it, tasks.end());
            fileHandler.saveTasks(tasks);
        }
    }
    
    void clearAll() {
        tasks.clear();
        fileHandler.saveTasks(tasks);
        nextId = 1;
    }
};