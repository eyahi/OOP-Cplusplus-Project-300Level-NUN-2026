/*
 * File: chatmessage.cpp
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Implementation for this component.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#include "chatmessage.h"

ChatMessage::ChatMessage()
    : m_delivered(false), m_read(false) {
}
ChatMessage::ChatMessage(const QString& id, const QString& senderId, const QString& senderName,
    const QString& content, const QDateTime& timestamp, const QString& replyTo)
    : m_id(id), m_senderId(senderId), m_senderName(senderName),
    m_content(content), m_timestamp(timestamp), m_replyToId(replyTo),
    m_delivered(false), m_read(false) {
}
