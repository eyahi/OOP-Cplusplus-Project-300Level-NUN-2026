#ifndef DIARY_ENTRY_H
#define DIARY_ENTRY_H

#include <QString>

// represents one diary entry with title, date, time, and content
class DiaryEntry {
public:
    DiaryEntry();
    DiaryEntry(const QString& title, const QString& date,
               const QString& time, const QString& content);

    // getters
    QString getTitle() const;
    QString getDate() const;
    QString getTime() const;
    QString getContent() const;

    // setters
    void setTitle(const QString& title);
    void setDate(const QString& date);
    void setTime(const QString& time);
    void setContent(const QString& content);

    // compare entries by title and date
    bool operator==(const DiaryEntry& other) const;

private:
    QString m_title;
    QString m_date;
    QString m_time;
    QString m_content;
};

#endif // DIARY_ENTRY_H
