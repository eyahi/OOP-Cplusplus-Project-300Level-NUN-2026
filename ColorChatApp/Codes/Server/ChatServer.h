/*
 * File: ChatServer.h
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Declares the TCP chat server class and protocol handlers.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QObject>
#include <QMap>
#include <QHash>
#include <QByteArray>
#include "user.h"
#include "chatroom.h"

class QTcpServer;
class QTcpSocket;
class QJsonObject;

class ChatServer : public QObject {
    Q_OBJECT

public:
    explicit ChatServer(quint16 port = 8080, QObject* parent = nullptr);
    ~ChatServer();

    // Server lifecycle
    bool start();
    void stop();
    bool isRunning() const;

private slots:
    // Connection management
    void onNewConnection();
    void onSocketReadyRead();
    void onSocketDisconnected();

private:
    // Message routing
    void routeMessage(QTcpSocket* socket, const QJsonObject& message);

    // Protocol handlers
    void handleRegister(QTcpSocket* socket, const QJsonObject& obj);
    void handleChatMessage(QTcpSocket* socket, const QJsonObject& obj);
    void handleCreateRoom(QTcpSocket* socket, const QJsonObject& obj);
    void handleJoinRoom(QTcpSocket* socket, const QJsonObject& obj);
    void handleLeaveRoom(QTcpSocket* socket, const QJsonObject& obj);


    // Helper methods
    void broadcastToRoom(const QString& roomId, const QJsonObject& obj);
    void sendToSocket(QTcpSocket* socket, const QJsonObject& obj);
    void sendToUser(const QString& userId, const QJsonObject& obj);
    void sendError(QTcpSocket* socket, const QString& code, const QString& message);

    QString userIdForSocket(QTcpSocket* socket) const;

    // Server state
    QTcpServer* m_server;
    quint16 m_port;
    QMap<QString, User> m_users;
    QMap<QString, ChatRoom> m_rooms;

    // Per-socket receive buffers for newline-delimited JSON
    QHash<QTcpSocket*, QByteArray> m_rxBuffers;

    // O(1) socket -> userId lookup (improves disconnect handling)
    QHash<QTcpSocket*, QString> m_socketToUserId;

    // Safety: cap message history per room to avoid unbounded memory growth
    static constexpr int kMaxRoomHistory = 500;
};

#endif // CHATSERVER_H