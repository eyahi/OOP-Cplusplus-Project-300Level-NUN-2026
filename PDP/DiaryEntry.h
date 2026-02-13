#ifndef DIARYENTRY_H
#define DIARYENTRY_H

#include <string>
using namespace std;

// Represents a single diary entry
class DiaryEntry {
private:
    string title;
    string date;
    string content;

public:
    // Constructor
    DiaryEntry(string t, string d, string c)
        : title(t), date(d), content(c) {}

    // Default constructor
    DiaryEntry() {}

    // Getters
    string getTitle() const { return title; }
    string getDate() const { return date; }
    string getContent() const { return content; }

    // Setters
    void setTitle(const string &t) { title = t; }
    void setDate(const string &d) { date = d; }
    void setContent(const string &c) { content = c; }
};

#endif
