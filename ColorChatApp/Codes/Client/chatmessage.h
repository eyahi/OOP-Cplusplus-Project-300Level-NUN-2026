/*
 * File: chatmessage.h
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Header declarations for this component.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#ifndef CHATMESSAGE_H
#define CHATMESSAGE_H

#include <QString>
#include <QDateTime>
#include <QMap>

class ChatMessage {
public:
    ChatMessage();
    ChatMessage(const QString& id, const QString& senderId, const QString& senderName,
        const QString& content, const QDateTime& timestamp, const QString& replyTo = "");

    // Getters
    QString id() const { return m_id; }
    QString senderId() const { return m_senderId; }
    QString senderName() const { return m_senderName; }
    QString content() const { return m_content; }
    QDateTime timestamp() const { return m_timestamp; }
    QString replyToId() const { return m_replyToId; }
    bool isDelivered() const { return m_delivered; }
    bool isRead() const { return m_read; }

    // Setters
    void setDelivered(bool delivered) { m_delivered = delivered; }
    void setRead(bool read) { m_read = read; }

private:
    QString m_id;
    QString m_senderId;
    QString m_senderName;
    QString m_content;
    QDateTime m_timestamp;
    QString m_replyToId;
    bool m_delivered;
    bool m_read;
};

#endif // CHATMESSAGE_H

