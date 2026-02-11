/*
 * File: bubblewidget.h
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Header declarations for this component.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#pragma once

#include <QFrame>
#include <QDateTime>

class QLabel;
class QFrame;

/**
 * @class BubbleWidget
 * @brief A custom widget representing a chat message bubble.
 * 
 * Handles rendering of the sender name, message content, timestamp, 
 * and optional reply preview. Supports both light and dark themes
 * and features a smooth entry animation.
 */
class BubbleWidget : public QFrame {
    Q_OBJECT
public:
    BubbleWidget(const QString& senderName,
                 const QString& text,
                 const QDateTime& timestamp,
                 bool mine,
                 const QString& replyPreview = QString(),
                 bool lightTheme = false,
                 QWidget* parent = nullptr);

    QString messageId() const { return m_messageId; }
    void setMessageId(const QString& id) { m_messageId = id; }

    // Make bubbles widen to a percentage of the chat area so medium messages stay on one line.
    void setMaxBubbleWidth(int px);


private:
    void recomputeWidth();

    QString m_messageId;
    QString m_previewText;
    QString m_senderName;
    QString m_bodyRaw;

    bool m_mine{false};
    int m_maxBubbleWidth{900};

    QFrame* m_bubble{nullptr};
    QLabel* m_sender{nullptr};
    QLabel* m_reply{nullptr};
    QLabel* m_body{nullptr};
    QLabel* m_meta{nullptr};
};
