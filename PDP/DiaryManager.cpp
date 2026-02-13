#include "DiaryManager.h"
#include <fstream>   // For file handling
#include <iostream>  // For console output (debug)

using namespace std;

// Constructor
DiaryManager::DiaryManager(string file) : fileName(file) {
    // Automatically load entries from file when the manager is created
    loadFromFile();
}

// Add a new entry
void DiaryManager::addEntry(const DiaryEntry& entry) {
    entries.push_back(entry);
}

// Get all entries
vector<DiaryEntry> DiaryManager::getEntries() const {
    return entries;
}

// Save entries to file
void DiaryManager::saveToFile() const {
    ofstream file(fileName);

    if (!file) {
        cerr << "Error: Could not open file for writing!" << endl;
        return;
    }

    for (const auto& entry : entries) {
        // Each entry is saved on separate lines
        file << entry.getTitle() << endl;
        file << entry.getDate() << endl;
        file << entry.getContent() << endl;
        file << "---" << endl; // Separator between entries
    }

    file.close();
}

// Load entries from file
void DiaryManager::loadFromFile() {
    ifstream file(fileName);

    if (!file) {
        // No file exists yet, so just return
        return;
    }

    entries.clear();

    string title, date, content, line;
    while (getline(file, title)) {
        getline(file, date);
        content = "";

        // Read all content lines until separator "---"
        while (getline(file, line) && line != "---") {
            content += line + "\n";
        }

        DiaryEntry entry(title, date, content);
        entries.push_back(entry);
    }

    file.close();
}
