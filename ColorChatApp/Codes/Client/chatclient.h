/*
 * File: chatclient.h
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Declares the chat client API used by the GUI.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QObject>
#include <QByteArray>
#include "userprofile.h"
#include "chatmessage.h"

class QTcpSocket;
class QJsonObject;

class ChatClient : public QObject {
    Q_OBJECT

public:
    explicit ChatClient(QObject* parent = nullptr);
    ~ChatClient();

    // Connection management
    void connectToServer(const QString& host, quint16 port);
    void disconnectFromServer();
    bool isConnected() const;

    // User profile access
    UserProfile* profile() { return m_profile; }
    const UserProfile* profile() const { return m_profile; }

    // User actions
    void registerUser(const QString& nickname);
    void updateProfile(const QString& nickname = "", const QString& avatar = "",
        const QString& about = "", const QString& status = "",
        const QString& statusEmoji = "", const QString& statusColor = "");

    // Messaging
    void sendMessage(const QString& roomId, const QString& content, const QString& replyToId = "");

    // Room management
    void createPrivateRoom(const QString& otherUserId);
    void createGroupRoom(const QStringList& userIds);
    void joinRoom(const QString& roomId);
    void leaveRoom(const QString& roomId);

signals:
    // Connection signals
    void connected();
    void disconnected();
    void connectionError(const QString& error);
    void protocolError(const QString& code, const QString& message);
    void registered(const QString& userId, const QString& globalRoomId);

    // Message signals
    void messageReceived(const QString& roomId, const ChatMessage& message, bool isMine);

    // Room signals
    void roomCreated(const QString& roomId, bool exists);
    void roomJoined(const QString& roomId, const QString& userId, const QString& userName);
    void roomLeft(const QString& roomId, const QString& userId, const QString& userName);

private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError();

private:
    // Message routing
    void routeMessage(const QJsonObject& message);

    // Protocol handlers
    void handleRegistered(const QJsonObject& obj);
    void handleMessage(const QJsonObject& obj);
    void handleRoomCreated(const QJsonObject& obj);
    void handleRoomJoined(const QJsonObject& obj);
    void handleRoomLeft(const QJsonObject& obj);

    void sendJson(const QJsonObject& obj);

    QTcpSocket* m_socket;
    UserProfile* m_profile;

    // Receive buffer for newline-delimited JSON
    QByteArray m_rxBuffer;
};

#endif // CHATCLIENT_H
