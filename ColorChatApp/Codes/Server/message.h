/*
 * File: message.h
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Header declarations for this component.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QDateTime>
#include <QMap>

/**
 * @class Message
 * @brief Represents a single chat message.
 * 
 * Contains content, sender info, timestamps, and metadata
 * like reactions or reply references.
 */
class Message {
public:
    Message();

    QString id;
    QString senderId;
    QString content;
    QDateTime timestamp;
    bool delivered;
    bool read;

    // Interactions (expandable)
    QMap<QString, QString> reactions;  // userId -> emoji
    QString replyToId;
};

#endif // MESSAGE_H

