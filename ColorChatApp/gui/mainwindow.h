/*
 * File: mainwindow.h
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Declares the main GUI window class.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#pragma once

#include <QMainWindow>
#include <QMap>
#include <QDate>

#include "chatclient.h"
#include "chatmessage.h"

class QListWidget;
class QListWidgetItem;
class QLabel;
class QLineEdit;
class QToolButton;
class QComboBox;
class QScrollArea;
class QVBoxLayout;
class QWidget;

struct UiMessage {
    ChatMessage msg;
    bool mine{false};
};

/**
 * @class MainWindow
 * @brief The primary user interface for the Colorful Chat application.
 * 
 * Manages the layout (sidebar and chat area), user networking interactions 
 * via ChatClient, theme switching, and room management.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;

private slots:
    void startConnectFlow();

    void onConnected();
    void onDisconnected();
    void onConnectionError(const QString& error);
    void onRegistered(const QString& userId, const QString& globalRoomId);

    void onMessageReceived(const QString& roomId, const ChatMessage& message, bool isMine);
    void onRoomJoined(const QString& roomId, const QString& userId, const QString& userName);
    void onRoomLeft(const QString& roomId, const QString& userId, const QString& userName);
    void onRoomCreated(const QString& roomId, bool exists);

    void onConversationChanged();
    void sendCurrentMessage();

    void startDmWithSelection();
    void startRoomWithSelection();
    void joinRoomFromInput();

    void onUserDoubleClicked(QListWidgetItem* item);

    void onThemeChanged(const QString& themeName);

    void toggleEmojiPanel();

private:
    void insertEmoji(const QString& emoji);
    void applyTheme(const QString& themeName);
    void updateWallpaper();
    void buildUi();
    void setupSidebar(class QSplitter* splitter);
    void setupChatArea(class QSplitter* splitter);
    void setupEmojiPanel(class QVBoxLayout* root);
    void setupComposer(class QVBoxLayout* root);
    void ensureRoomExists(const QString& roomId, const QString& title);
    void selectRoom(const QString& roomId);
    void renderRoom(const QString& roomId);
    void appendMessageToView(const QString& roomId, const ChatMessage& message, bool mine);

    void setSidebarGlow(QListWidgetItem* item, bool glow);

    // helpers
    void clearLayout(QVBoxLayout* layout);
    int computeMaxBubbleWidth() const;
    void updateAllBubbleWidths();

private:
    ChatClient* m_client{nullptr};

    // UI
    QListWidget* m_conversations{nullptr};
    QListWidget* m_users{nullptr};
    QLabel* m_title{nullptr};
    QLabel* m_profileLine{nullptr};
    QLabel* m_connChip{nullptr};
    QComboBox* m_theme{nullptr};

    QWidget* m_messagesContainer{nullptr};
    QScrollArea* m_scroll{nullptr};
    QVBoxLayout* m_messagesLayout{nullptr};

    QLineEdit* m_input{nullptr};
    QToolButton* m_sendBtn{nullptr};

    // Emoji UI
    QToolButton* m_emojiBtn{nullptr};
    QWidget* m_emojiPanel{nullptr};

    bool m_isLightTheme{false};

    // DM/Room controls
    QToolButton* m_dmBtn{nullptr};
    QToolButton* m_roomBtn{nullptr};
    QLineEdit* m_roomNameInput{nullptr};
    QLineEdit* m_joinRoomInput{nullptr};
    QToolButton* m_joinRoomBtn{nullptr};

    // State
    QString m_userId;
    QString m_globalRoomId{"global"};
    QString m_currentRoomId{"global"};

    QMap<QString, QList<UiMessage>> m_history;
    QMap<QString, QListWidgetItem*> m_roomItem;
    QMap<QString, QDate> m_lastRenderedDate;

    QMap<QString, QString> m_onlineUsers; // userId -> userName

    // Friendly room titles (local-only)
    QMap<QString, QString> m_customRoomTitle; // roomId -> title
    QString m_pendingRoomTitle;
    QStringList m_pendingRoomUsers;

    // room labels
    QString roomTitleForId(const QString& roomId) const;
    void refreshOnlineUsersList();
};
