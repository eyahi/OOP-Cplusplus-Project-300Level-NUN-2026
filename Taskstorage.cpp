#include <string>
#include <fstream>
#include <vector>

using namespace std;


class Task {
private:
    int id;
    string description;
    string dueDate;
    bool completed;

public:
    Task(int taskId, string desc, string date, bool comp = false)
        : id(taskId), description(desc), dueDate(date), completed(comp) {}
    
    int getId() const { return id; }
    string getDescription() const { return description; }
    string getDueDate() const { return dueDate; }
    bool isCompleted() const { return completed; }
    
    void setCompleted(bool status) { completed = status; }
    void setDescription(const string& desc) { description = desc; }
    void setDueDate(const string& date) { dueDate = date; }
    
    string toFileFormat() const {
        return to_string(id) + "|" + description + "|" + dueDate + "|" + 
               (completed ? "1" : "0");
    }
};


class FileHandler {
private:
    string filename;

public:
    FileHandler(const string& fname = "tasks.txt") : filename(fname) {}
    
    void saveTasks(const vector<Task>& tasks) {
        ofstream file(filename);
        if (!file.is_open()) return;
        
        for (const auto& task : tasks) {
            file << task.toFileFormat() << '\n';
        }
        file.close();
    }
    
    vector<Task> loadTasks() {
        vector<Task> tasks;
        ifstream file(filename);
        
        if (!file.is_open()) return tasks;
        
        string line;
        while (getline(file, line)) {
            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);
            size_t pos3 = line.rfind('|');
            
            if (pos1 != string::npos && pos2 != string::npos && 
                pos3 != string::npos && pos1 < pos2 && pos2 < pos3) {
                try {
                    int id = stoi(line.substr(0, pos1));
                    string desc = line.substr(pos1 + 1, pos2 - pos1 - 1);
                    string date = line.substr(pos2 + 1, pos3 - pos2 - 1);
                    bool completed = (line.substr(pos3 + 1) == "1");
                    
                    tasks.push_back(Task(id, desc, date, completed));
                } catch (...) {
                    continue;
                }
            }
        }
        
        file.close();
        return tasks;
    }
};