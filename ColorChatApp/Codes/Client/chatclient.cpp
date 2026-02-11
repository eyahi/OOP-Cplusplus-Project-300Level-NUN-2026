/*
 * File: chatclient.cpp
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Implements the TCP chat client networking and protocol parsing.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */


#include "chatclient.h"

#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

ChatClient::ChatClient(QObject* parent)
    : QObject(parent) {
    
    m_socket = new QTcpSocket(this);
    m_profile = new UserProfile(this);
    
    connect(m_socket, &QTcpSocket::connected, this, &ChatClient::onConnected);
    connect(m_socket, &QTcpSocket::disconnected, this, &ChatClient::onDisconnected);
    connect(m_socket, &QTcpSocket::readyRead, this, &ChatClient::onReadyRead);
    connect(m_socket, &QTcpSocket::errorOccurred, this, &ChatClient::onError);
}

ChatClient::~ChatClient() {
    disconnectFromServer();
}

void ChatClient::connectToServer(const QString& host, quint16 port) {
    qDebug() << "Connecting to" << host << ":" << port;
    m_rxBuffer.clear();
    m_socket->connectToHost(host, port);
}

void ChatClient::disconnectFromServer() {
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->disconnectFromHost();
    }
}

bool ChatClient::isConnected() const {
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void ChatClient::registerUser(const QString& nickname) {
    // Sends a registration request with the current profile data.
    // Respect any profile fields already set by the UI.
    if (!nickname.isEmpty()) m_profile->setNickname(nickname);
    if (m_profile->about().isEmpty()) m_profile->setAbout("Qt Chat User");
    if (m_profile->status().isEmpty()) m_profile->setStatus("Online");
    if (m_profile->statusColor().isEmpty()) m_profile->setStatusColor("#2ecc71");
    
    QJsonObject msg;
    msg["type"] = "register";
    msg["userId"] = m_profile->userId();
    msg["nickname"] = m_profile->nickname();
    msg["about"] = m_profile->about();
    msg["status"] = m_profile->status();
    msg["statusColor"] = m_profile->statusColor();
    
    sendJson(msg);
}

void ChatClient::updateProfile(const QString& nickname, const QString& avatar,
                               const QString& about, const QString& status,
                               const QString& statusEmoji, const QString& statusColor) {
    QJsonObject msg;
    msg["type"] = "updateProfile";
    msg["userId"] = m_profile->userId();
    
    if (!nickname.isEmpty()) {
        m_profile->setNickname(nickname);
        msg["nickname"] = nickname;
    }
    if (!about.isEmpty()) {
        m_profile->setAbout(about);
        msg["about"] = about;
    }
    if (!status.isEmpty()) {
        m_profile->setStatus(status);
        msg["status"] = status;
    }
    if (!statusColor.isEmpty()) {
        m_profile->setStatusColor(statusColor);
        msg["statusColor"] = statusColor;
    }
    
    sendJson(msg);
}

void ChatClient::sendMessage(const QString& roomId, const QString& content, const QString& replyToId) {
    if (!isConnected()) {
        emit protocolError("not_connected", "Not connected to server");
        return;
    }
    if (roomId.isEmpty() || content.trimmed().isEmpty()) {
        emit protocolError("bad_request", "Room ID and message content are required");
        return;
    }

    QJsonObject msg;
    msg["type"] = "message";
    msg["chatId"] = roomId;
    msg["senderId"] = m_profile->userId();
    msg["content"] = content;
    msg["replyToId"] = replyToId;
    
    sendJson(msg);
}

void ChatClient::createPrivateRoom(const QString& otherUserId) {
    if (!isConnected()) {
        emit protocolError("not_connected", "Not connected to server");
        return;
    }
    if (otherUserId.isEmpty()) {
        emit protocolError("bad_request", "Other user ID is required");
        return;
    }

    QJsonObject msg;
    msg["type"] = "createRoom";
    msg["creatorId"] = m_profile->userId();
    msg["roomType"] = "private";
    
    QJsonArray participants;
    participants.append(otherUserId);
    msg["participants"] = participants;
    
    sendJson(msg);
}

void ChatClient::createGroupRoom(const QStringList& userIds) {
    if (!isConnected()) {
        emit protocolError("not_connected", "Not connected to server");
        return;
    }
    if (userIds.isEmpty()) {
        emit protocolError("bad_request", "At least one participant is required");
        return;
    }

    QJsonObject msg;
    msg["type"] = "createRoom";
    msg["creatorId"] = m_profile->userId();
    msg["roomType"] = "group";
    
    QJsonArray participants;
    for (const QString& userId : userIds) {
        participants.append(userId);
    }
    msg["participants"] = participants;
    
    sendJson(msg);
}

void ChatClient::joinRoom(const QString& roomId) {
    if (!isConnected()) {
        emit protocolError("not_connected", "Not connected to server");
        return;
    }
    if (roomId.isEmpty()) {
        emit protocolError("bad_request", "Room ID is required");
        return;
    }

    QJsonObject msg;
    msg["type"] = "joinRoom";
    msg["userId"] = m_profile->userId();
    msg["roomId"] = roomId;
    
    sendJson(msg);
}

void ChatClient::leaveRoom(const QString& roomId) {
    if (!isConnected()) {
        emit protocolError("not_connected", "Not connected to server");
        return;
    }
    if (roomId.isEmpty()) {
        emit protocolError("bad_request", "Room ID is required");
        return;
    }

    QJsonObject msg;
    msg["type"] = "leaveRoom";
    msg["userId"] = m_profile->userId();
    msg["roomId"] = roomId;
    
    sendJson(msg);
}

void ChatClient::onConnected() {
    qDebug() << "Connected to server";
    emit connected();
}

void ChatClient::onDisconnected() {
    qDebug() << "Disconnected from server";
    emit disconnected();
}

void ChatClient::onReadyRead() {
    // Buffers incoming data and processes complete newline-delimited JSON messages.
    // Newline-delimited JSON framing.
    m_rxBuffer.append(m_socket->readAll());

    // Safety: prevent unbounded memory growth if server sends huge data without newlines
    if (m_rxBuffer.size() > 5 * 1024 * 1024) { 
        m_socket->disconnectFromHost();
        return;
    }

    int newlineIndex = -1;
    while ((newlineIndex = m_rxBuffer.indexOf('\n')) != -1) {
        QByteArray line = m_rxBuffer.left(newlineIndex);
        m_rxBuffer.remove(0, newlineIndex + 1);
        line = line.trimmed();
        if (line.isEmpty()) continue;

        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(line, &err);
        if (err.error != QJsonParseError::NoError || !doc.isObject()) {
            qDebug() << "Invalid JSON from server:" << err.errorString();
            continue;
        }
        routeMessage(doc.object());
    }
}

void ChatClient::onError() {
    qDebug() << "Socket error:" << m_socket->errorString();
    emit connectionError(m_socket->errorString());
}

void ChatClient::routeMessage(const QJsonObject& message) {
    // Maps incoming server messages to specific signals for the UI components.
    QString type = message["type"].toString();
    
    if (type == "registered") {
        handleRegistered(message);
    } else if (type == "message") {
        handleMessage(message);
    } else if (type == "roomCreated") {
        handleRoomCreated(message);
    } else if (type == "roomJoined") {
        handleRoomJoined(message);
    } else if (type == "roomLeft") {
        handleRoomLeft(message);
    } else if (type == "error") {
        const QString code = message.value("code").toString();
        const QString msg = message.value("message").toString();
        emit protocolError(code, msg);
    } else {
        emit protocolError("unknown_message_type", "Unknown message type from server: " + type);
    }
}

void ChatClient::handleRegistered(const QJsonObject& obj) {
    QString userId = obj["userId"].toString();
    QString globalRoomId = obj["globalRoomId"].toString();
    
    qDebug() << "Registered as:" << m_profile->nickname();
    emit registered(userId, globalRoomId);
}

void ChatClient::handleMessage(const QJsonObject& obj) {
    QString roomId = obj["chatId"].toString();
    QString msgId = obj["messageId"].toString();
    QString senderId = obj["senderId"].toString();
    QString senderName = obj["senderName"].toString();
    QString content = obj["content"].toString();
    QDateTime timestamp = QDateTime::fromString(obj["timestamp"].toString(), Qt::ISODate);
    QString replyTo = obj["replyToId"].toString();
    
    ChatMessage msg(msgId, senderId, senderName, content, timestamp, replyTo);
    bool isMine = (senderId == m_profile->userId());
    
    emit messageReceived(roomId, msg, isMine);
}

void ChatClient::handleRoomCreated(const QJsonObject& obj) {
    QString roomId = obj["roomId"].toString();
    bool exists = obj["exists"].toBool();
    
    qDebug() << "Room created:" << roomId;
    emit roomCreated(roomId, exists);
}

void ChatClient::handleRoomJoined(const QJsonObject& obj) {
    QString roomId = obj["roomId"].toString();
    QString userId = obj["userId"].toString();
    QString userName = obj["userName"].toString();
    
    emit roomJoined(roomId, userId, userName);
}

void ChatClient::handleRoomLeft(const QJsonObject& obj) {
    QString roomId = obj["roomId"].toString();
    QString userId = obj["userId"].toString();
    QString userName = obj["userName"].toString();
    
    emit roomLeft(roomId, userId, userName);
}

void ChatClient::sendJson(const QJsonObject& obj) {
    if (!isConnected()) {
        emit protocolError("not_connected", "Not connected to server");
        return;
    }

    QJsonDocument doc(obj);
    m_socket->write(doc.toJson(QJsonDocument::Compact));
    m_socket->write("\n");
    m_socket->flush();
}
