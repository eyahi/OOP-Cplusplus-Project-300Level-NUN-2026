/*
 * File: userprofile.h
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Header declarations for this component.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#ifndef USERPROFILE_H
#define USERPROFILE_H

#include <QObject>
#include <QString>
#include <QDateTime>

class UserProfile : public QObject {
    Q_OBJECT

public:
    explicit UserProfile(QObject* parent = nullptr);

    // Getters
    QString userId() const { return m_userId; }
    QString nickname() const { return m_nickname; }
    QString about() const { return m_about; }
    QString status() const { return m_status; }
    QString statusColor() const { return m_statusColor; }
    QDateTime lastSeen() const { return m_lastSeen; }
    bool isOnline() const { return m_online; }

    // Setters
    void setUserId(const QString& id);
    void setNickname(const QString& name);
    void setAbout(const QString& about);
    void setStatus(const QString& status);
    void setStatusColor(const QString& color);
    void setLastSeen(const QDateTime& time);
    void setOnline(bool online);

signals:
    void profileUpdated();
    void statusChanged(const QString& status);

private:
    QString m_userId;
    QString m_nickname;
    QString m_about;
    QString m_status;
    QString m_statusColor;
    QDateTime m_lastSeen;
    bool m_online;
};

#endif // USERPROFILE_H
