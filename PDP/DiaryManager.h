#ifndef DIARYMANAGER_H
#define DIARYMANAGER_H

#include <vector>
#include <string>
#include "DiaryEntry.h"
using namespace std;

// This class manages multiple diary entries
class DiaryManager {
private:
    vector<DiaryEntry> entries; // List of all diary entries
    string fileName;            // File to save/load entries

public:
    // Constructor
    DiaryManager(string file = "diary.txt");

    // Add a new diary entry
    void addEntry(const DiaryEntry& entry);

    // Get all entries
    vector<DiaryEntry> getEntries() const;

    // Save entries to file
    void saveToFile() const;

    // Load entries from file
    void loadFromFile();
};

#endif
