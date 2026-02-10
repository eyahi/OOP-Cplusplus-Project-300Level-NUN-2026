#include "DiaryStorage.h"
#include <QFile>
#include <QTextStream>

DiaryStorage::DiaryStorage(const QString& dataFilePath)
    : m_dataFile(dataFilePath) {}

// load all entries from the text file
bool DiaryStorage::loadEntries(QList<DiaryEntry>& entries) {
    QFile file(m_dataFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false; // file doesn't exist yet or can't be opened
    }

    QTextStream in(&file);
    entries.clear();

    // parse the file line by line
    while (!in.atEnd()) {
        QString line = in.readLine();

        if (line.startsWith("TITLE:")) {
            QString title = line.mid(6).trimmed();

            // read date line
            line = in.readLine();
            QString date;
            if (line.startsWith("DATE:")) {
                date = line.mid(5).trimmed();
            }

            // read time line
            line = in.readLine();
            QString time;
            if (line.startsWith("TIME:")) {
                time = line.mid(5).trimmed();
            }

            // read content until we hit the end marker
            QString content;
            while (!in.atEnd()) {
                line = in.readLine();
                if (line == "---END---") {
                    break;
                }
                content += line + "\n";
            }

            // create entry and add to list
            DiaryEntry entry(title, date, time, content.trimmed());
            entries.append(entry);
        }
    }

    file.close();
    return true;
}

// save all entries to the text file
bool DiaryStorage::saveEntries(const QList<DiaryEntry>& entries) {
    QFile file(m_dataFile);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false; // couldn't open file for writing
    }

    QTextStream out(&file);

    // write each entry with markers so we can parse it later
    for (const auto& entry : entries) {
        out << "TITLE:" << entry.getTitle() << "\n";
        out << "DATE:" << entry.getDate() << "\n";
        out << "TIME:" << entry.getTime() << "\n";
        out << entry.getContent() << "\n";
        out << "---END---\n";
    }

    file.close();
    return true;
}
