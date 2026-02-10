#include "DiaryEntry.h"

// default constructor
DiaryEntry::DiaryEntry() {}

// constructor with all the entry data
DiaryEntry::DiaryEntry(const QString& title, const QString& date,
                       const QString& time, const QString& content)
    : m_title(title), m_date(date), m_time(time), m_content(content) {}

// getters - pretty straightforward
QString DiaryEntry::getTitle() const {
    return m_title;
}

QString DiaryEntry::getDate() const {
    return m_date;
}

QString DiaryEntry::getTime() const {
    return m_time;
}

QString DiaryEntry::getContent() const {
    return m_content;
}

// setters
void DiaryEntry::setTitle(const QString& title) {
    m_title = title;
}

void DiaryEntry::setDate(const QString& date) {
    m_date = date;
}

void DiaryEntry::setTime(const QString& time) {
    m_time = time;
}

void DiaryEntry::setContent(const QString& content) {
    m_content = content;
}

// two entries are equal if they have same title and date
bool DiaryEntry::operator==(const DiaryEntry& other) const {
    return m_title == other.m_title && m_date == other.m_date;
}
