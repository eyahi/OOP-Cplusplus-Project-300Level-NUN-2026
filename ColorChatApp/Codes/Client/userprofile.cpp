/*
 * File: userprofile.cpp
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Implementation for this component.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#include "userprofile.h"

#include <QUuid>

UserProfile::UserProfile(QObject* parent)
    : QObject(parent), m_online(false) {
    m_userId = QUuid::createUuid().toString();
}
void UserProfile::setUserId(const QString& id) {
    m_userId = id;
}
void UserProfile::setNickname(const QString& name) {
    if (m_nickname != name) {
        m_nickname = name;
        emit profileUpdated();
    }
}
void UserProfile::setAbout(const QString& about) {
    if (m_about != about) {
        m_about = about;
        emit profileUpdated();
    }
}
void UserProfile::setStatus(const QString& status) {
    if (m_status != status) {
        m_status = status;
        emit statusChanged(status);
        emit profileUpdated();
    }
}
void UserProfile::setStatusColor(const QString& color) {
    m_statusColor = color;
}
void UserProfile::setLastSeen(const QDateTime& time) {
    m_lastSeen = time;
}
void UserProfile::setOnline(bool online) {
    m_online = online;
}
