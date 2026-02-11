/*
 * File: chatroom.h
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Header declarations for this component.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#ifndef CHATROOM_H
#define CHATROOM_H

#include <QString>
#include <QSet>
#include <QList>
#include "message.h"

/**
 * @class ChatRoom
 * @brief Represents a chat room (Global, DM, or Group).
 * 
 * Manages a set of participants and a history of messages.
 */
class ChatRoom {
public:
    ChatRoom();

    QString id;
    QString roomType;  // "private", "group", "global"
    QSet<QString> participants;
    QList<Message> messages;

    // Room features (expandable)
    QStringList pinnedMessages;
};

#endif // CHATROOM_H