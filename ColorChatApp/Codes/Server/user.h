/*
 * File: user.h
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Header declarations for this component.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#ifndef USER_H
#define USER_H

#include <QString>
#include <QDateTime>

class QTcpSocket;

/**
 * @class User
 * @brief Represents a user in the chat system.
 * 
 * Stores connection details (socket), identity (id, nickname),
 * and profile information (status, about).
 */
class User {
public:
    User();

    // Basic info
    QString id;
    QString nickname;
    QTcpSocket* socket;
    QDateTime lastSeen;

    // Profile info
    QString status;
    QString statusColor;
    QString about;
};

#endif // USER_H

