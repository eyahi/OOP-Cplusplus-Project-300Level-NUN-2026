/*
 * File: ChatServer.cpp
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Implements the TCP chat server and message routing.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#include "ChatServer.h"
#include "user.h"
#include "chatroom.h"
#include "message.h"
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUuid>
#include <QDebug>

User::User() : socket(nullptr) {
}

Message::Message() : delivered(false), read(false) {
}

ChatRoom::ChatRoom() {
}

ChatServer::ChatServer(quint16 port, QObject* parent)
    : QObject(parent), m_port(port) {

    m_server = new QTcpServer(this);

    // Create default global room
    ChatRoom globalRoom;
    globalRoom.id = "global";
    globalRoom.roomType = "global";
    m_rooms["global"] = globalRoom;

    connect(m_server, &QTcpServer::newConnection,
        this, &ChatServer::onNewConnection);
}

ChatServer::~ChatServer() {
    stop();
}

bool ChatServer::start() {
    if (m_server->listen(QHostAddress::Any, m_port)) {
        qDebug() << "Server started on port" << m_port;
        qDebug() << "Architecture: Room-based routing";
        return true;
    }
    qDebug() << "Failed to start:" << m_server->errorString();
    return false;
}

void ChatServer::stop() {
    if (m_server->isListening()) {
        m_server->close();
        qDebug() << "Server stopped";
    }
}

bool ChatServer::isRunning() const {
    return m_server->isListening();
}

void ChatServer::onNewConnection() {
    QTcpSocket* socket = m_server->nextPendingConnection();
    qDebug() << "New connection:" << socket->peerAddress().toString();

    // Initialize per-socket receive buffer
    m_rxBuffers.insert(socket, QByteArray());

    connect(socket, &QTcpSocket::readyRead,
        this, &ChatServer::onSocketReadyRead);
    connect(socket, &QTcpSocket::disconnected,
        this, &ChatServer::onSocketDisconnected);
}

void ChatServer::onSocketReadyRead() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    // Newline-delimited JSON framing.
    // This avoids broken parsing when multiple JSON objects are coalesced in one TCP packet.
    // We append new data to the buffer and process only complete, newline-terminated messages.
    QByteArray& buffer = m_rxBuffers[socket];
    buffer.append(socket->readAll());

    // Safety: Cap buffer to 1MB to prevent memory exhaustion
    if (buffer.size() > 1024 * 1024) {
        qDebug() << "Buffer overflow from" << socket->peerAddress().toString();
        socket->disconnectFromHost();
        return;
    }

    int newlineIndex = -1;
    while ((newlineIndex = buffer.indexOf('\n')) != -1) {
        QByteArray line = buffer.left(newlineIndex);
        buffer.remove(0, newlineIndex + 1);
        line = line.trimmed();
        if (line.isEmpty()) continue;

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(line, &err);
        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            qDebug() << "Invalid JSON from" << socket->peerAddress().toString() << ":" << err.errorString();
            continue;
        }
        routeMessage(socket, doc.object());
    }
}

void ChatServer::onSocketDisconnected() {
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    // Drop per-socket buffer
    m_rxBuffers.remove(socket);

    const QString userId = userIdForSocket(socket);
    m_socketToUserId.remove(socket);
    if (!userId.isEmpty() && m_users.contains(userId)) {
        m_users[userId].lastSeen = QDateTime::currentDateTime();
        QString nickname = m_users[userId].nickname;
        m_users[userId].socket = nullptr;

        // Remove from all rooms and notify remaining participants
        for (auto it = m_rooms.begin(); it != m_rooms.end(); ++it) {
            ChatRoom& room = it.value();
            if (!room.participants.contains(userId)) continue;

            room.participants.remove(userId);

            QJsonObject notification;
            notification["type"] = "roomLeft";
            notification["roomId"] = room.id;
            notification["userId"] = userId;
            notification["userName"] = nickname;

            broadcastToRoom(room.id, notification);
        }

        qDebug() << "User disconnected:" << nickname;
    }

    socket->deleteLater();

}

void ChatServer::routeMessage(QTcpSocket* socket, const QJsonObject& message) {
    // Dispatches the parsed JSON message to the appropriate handler based on the "type" field.
    QString type = message["type"].toString();

    if (type == "register") {
        handleRegister(socket, message);
    }
    else if (type == "message") {
        handleChatMessage(socket, message);
    }
    else if (type == "createRoom") {
        handleCreateRoom(socket, message);
    }
    else if (type == "joinRoom") {
        handleJoinRoom(socket, message);
    }
    else if (type == "leaveRoom") {
        handleLeaveRoom(socket, message);
    }
    else {
        sendError(socket, "unknown_message_type", "Unknown message type: " + type);
    }
}

void ChatServer::handleRegister(QTcpSocket* socket, const QJsonObject& obj) {
    // Registers a new user, sends a snapshot of the current room state, and notifies existing participants.
    // VALIDATION_REGISTER
    const QString userId = obj.value("userId").toString();
    const QString nickname = obj.value("nickname").toString();
    if (userId.isEmpty() || nickname.isEmpty()) {
        sendError(socket, "bad_request", "Missing required fields: userId and nickname");
        return;
    }
    // If user already exists, ensure we don't have a duplicate connection.
    if (m_users.contains(userId)) {
        User& existing = m_users[userId];
        if (existing.socket && existing.socket != socket) {
            existing.socket->disconnectFromHost();
        }
    }

    User user;
    user.id = userId;
    user.nickname = nickname;
    user.socket = socket;
    user.lastSeen = QDateTime::currentDateTime();

    // Optional profile fields
    user.status = obj.value("status").toString("Online");
    user.statusColor = obj.value("statusColor").toString("#2ecc71");
    user.about = obj.value("about").toString("");

    // Snapshot of who is already in the global room (for the newly connected user)
    QSet<QString> existingParticipants = m_rooms["global"].participants;

    m_users[user.id] = user;
    m_socketToUserId.insert(socket, user.id);
    m_rooms["global"].participants.insert(user.id);

    // Send confirmation
    QJsonObject response;
    response["type"] = "registered";
    response["userId"] = user.id;
    response["globalRoomId"] = "global";
    sendToSocket(socket, response);

    // Send the existing participants to the new user so the client can build an online list
    for (const QString& existingId : existingParticipants) {
        if (!m_users.contains(existingId)) continue;
        QJsonObject snapshot;
        snapshot["type"] = "roomJoined";
        snapshot["roomId"] = "global";
        snapshot["userId"] = existingId;
        snapshot["userName"] = m_users[existingId].nickname;
        sendToSocket(socket, snapshot);
    }

    // Notify everyone (including the new user) that this user joined
    QJsonObject joined;
    joined["type"] = "roomJoined";
    joined["roomId"] = "global";
    joined["userId"] = user.id;
    joined["userName"] = user.nickname;
    broadcastToRoom("global", joined);

    qDebug() << "Registered:" << user.nickname << "-> global room";
}

void ChatServer::handleChatMessage(QTcpSocket* socket, const QJsonObject& obj) {
    // Validates the message, stores it in history, and broadcasts it to all room participants.
    QString roomId = obj["chatId"].toString();
    QString senderId = obj["senderId"].toString();
    QString content = obj["content"].toString();

    // Validate inputs
    if (roomId.isEmpty() || senderId.isEmpty() || content.isEmpty()) {
        sendError(socket, "bad_request", "Missing required fields for message");
        return;
    }

    if (!m_rooms.contains(roomId)) {
        sendError(socket, "not_found", "Room not found: " + roomId);
        return;
    }

    if (!m_users.contains(senderId)) {
        sendError(socket, "unauthorized", "Unknown senderId");
        return;
    }

    if (!m_rooms[roomId].participants.contains(senderId)) {
        sendError(socket, "forbidden", "Sender is not a participant of room: " + roomId);
        return;
    }

    // Create message
    Message msg;
    msg.id = QUuid::createUuid().toString();
    msg.senderId = senderId;
    msg.content = content;
    msg.timestamp = QDateTime::currentDateTime();
    msg.replyToId = obj.value("replyToId").toString("");

    m_rooms[roomId].messages.append(msg);
    while (m_rooms[roomId].messages.size() > kMaxRoomHistory) {
        m_rooms[roomId].messages.removeFirst();
    }

    // Broadcast to room participants
    QJsonObject broadcast;
    broadcast["type"] = "message";
    broadcast["chatId"] = roomId;
    broadcast["messageId"] = msg.id;
    broadcast["senderId"] = msg.senderId;
    broadcast["senderName"] = m_users[senderId].nickname;
    broadcast["content"] = msg.content;
    broadcast["timestamp"] = msg.timestamp.toString(Qt::ISODate);
    broadcast["replyToId"] = msg.replyToId;

    broadcastToRoom(roomId, broadcast);

    qDebug() << "Message [" << roomId << "]:" << m_users[senderId].nickname
        << "?" << m_rooms[roomId].participants.size() << "users";
}

void ChatServer::handleCreateRoom(QTcpSocket* socket, const QJsonObject& obj) {
    const QString creatorId = obj.value("creatorId").toString();
    QString roomType = obj["roomType"].toString("group");
    QJsonArray participantArray = obj["participants"].toArray();

    if (creatorId.isEmpty() || !m_users.contains(creatorId)) {
        sendError(socket, "unauthorized", "Unknown creatorId");
        return;
    }

    // Build participant list
    QSet<QString> participants;
    participants.insert(creatorId);
    for (const QJsonValue& val : participantArray) {
        participants.insert(val.toString());
    }

    // Generate room ID
    QString roomId;
    if (roomType == "private" && participants.size() == 2) {
        // Private room: consistent ID regardless of creation order
        QStringList ids = participants.values();
        ids.sort();
        roomId = "private_" + ids[0] + "_" + ids[1];
    }
    else {
        roomId = "room_" + QUuid::createUuid().toString();
    }

    // Check if room already exists
    if (m_rooms.contains(roomId)) {
        QJsonObject response;
        response["type"] = "roomCreated";
        response["roomId"] = roomId;
        response["exists"] = true;
        sendToSocket(socket, response);
        return;
    }

    // Create new room
    ChatRoom room;
    room.id = roomId;
    room.roomType = roomType;
    room.participants = participants;
    m_rooms[roomId] = room;

    // Notify all participants
    QJsonObject response;
    response["type"] = "roomCreated";
    response["roomId"] = roomId;
    response["exists"] = false;

    for (const QString& userId : participants) {
        sendToUser(userId, response);
    }

    qDebug() << "Room created:" << roomId << "[" << roomType << "]"
        << participants.size() << "users";
}

void ChatServer::handleJoinRoom(QTcpSocket* socket, const QJsonObject& obj) {
    QString userId = obj["userId"].toString();
    QString roomId = obj["roomId"].toString();

    if (userId.isEmpty() || roomId.isEmpty()) {
        sendError(socket, "bad_request", "Missing userId or roomId");
        return;
    }

    if (!m_rooms.contains(roomId)) {
        sendError(socket, "not_found", "Room not found: " + roomId);
        return;
    }

    if (!m_users.contains(userId)) {
        sendError(socket, "unauthorized", "Unknown userId");
        return;
    }

    m_rooms[roomId].participants.insert(userId);

    QJsonObject notification;
    notification["type"] = "roomJoined";
    notification["roomId"] = roomId;
    notification["userId"] = userId;
    notification["userName"] = m_users[userId].nickname;

    broadcastToRoom(roomId, notification);
    qDebug() << "User" << m_users[userId].nickname << "joined room" << roomId;
}

void ChatServer::handleLeaveRoom(QTcpSocket* socket, const QJsonObject& obj) {
    QString userId = obj["userId"].toString();
    QString roomId = obj["roomId"].toString();

    if (userId.isEmpty() || roomId.isEmpty()) {
        sendError(socket, "bad_request", "Missing userId or roomId");
        return;
    }

    if (!m_rooms.contains(roomId)) {
        sendError(socket, "not_found", "Room not found: " + roomId);
        return;
    }

    if (!m_users.contains(userId)) {
        sendError(socket, "unauthorized", "Unknown userId");
        return;
    }

    m_rooms[roomId].participants.remove(userId);

    QJsonObject notification;
    notification["type"] = "roomLeft";
    notification["roomId"] = roomId;
    notification["userId"] = userId;
    notification["userName"] = m_users[userId].nickname;

    broadcastToRoom(roomId, notification);
    qDebug() << "User" << m_users[userId].nickname << "left room" << roomId;
}

void ChatServer::broadcastToRoom(const QString& roomId, const QJsonObject& obj) {
    if (!m_rooms.contains(roomId)) return;

    for (const QString& uid : m_rooms[roomId].participants) {
        sendToUser(uid, obj);
    }
}

void ChatServer::sendToSocket(QTcpSocket* socket, const QJsonObject& obj) {
    if (!socket || socket->state() != QAbstractSocket::ConnectedState) return;

    QJsonDocument doc(obj);
    // Newline-delimit JSON messages for safe framing
    socket->write(doc.toJson(QJsonDocument::Compact));
    socket->write("\n");
    socket->flush();
}

void ChatServer::sendToUser(const QString& userId, const QJsonObject& obj) {
    const auto it = m_users.constFind(userId);
    if (it == m_users.constEnd()) return;
    if (!it.value().socket) return;

    sendToSocket(it.value().socket, obj);
}

void ChatServer::sendError(QTcpSocket* socket, const QString& code, const QString& message) {
    QJsonObject err;
    err["type"] = "error";
    err["code"] = code;
    err["message"] = message;
    sendToSocket(socket, err);
}

QString ChatServer::userIdForSocket(QTcpSocket* socket) const {
    return m_socketToUserId.value(socket);
}
