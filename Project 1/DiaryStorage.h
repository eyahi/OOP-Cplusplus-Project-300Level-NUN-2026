#ifndef DIARY_STORAGE_H
#define DIARY_STORAGE_H

#include "DiaryEntry.h"
#include <QList>
#include <QString>

// handles reading and writing entries to the text file
class DiaryStorage {
public:
    DiaryStorage(const QString& dataFilePath);

    // load entries from file
    bool loadEntries(QList<DiaryEntry>& entries);

    // save entries to file
    bool saveEntries(const QList<DiaryEntry>& entries);

private:
    QString m_dataFile;
};

#endif // DIARY_STORAGE_H
