#include "chatserver.h"
#include <QDebug>

ChatServer::ChatServer(QObject *parent)
    : QTcpServer(parent)
{
    // Start listening for client connections
    if (listen(QHostAddress::Any, 1234)) {
        qDebug() << "Chat server started on port 1234";
    } else {
        qDebug() << "Failed to start chat server";
    }
}

void ChatServer::incomingConnection(qintptr socketDescriptor)
{
    // Create a new socket for the connected client
    QTcpSocket *client = new QTcpSocket(this);
    client->setSocketDescriptor(socketDescriptor);

    // Connect signals for message handling and disconnection
    connect(client, &QTcpSocket::readyRead,
            this, &ChatServer::onReadyRead);
    connect(client, &QTcpSocket::disconnected,
            this, &ChatServer::onDisconnected);

    // Store client in the list
    m_clients.push_back(client);
}

void ChatServer::onReadyRead()
{
    // Identify which client sent the message
    QTcpSocket *senderClient =
        qobject_cast<QTcpSocket*>(sender());
    if (!senderClient)
        return;

    // Read incoming message
    const QByteArray data = senderClient->readAll();

    // Forward message to all other connected clients
    for (QTcpSocket *client : m_clients)
    {
        if (client != senderClient)
        {
            client->write(data);
        }
    }
}

void ChatServer::onDisconnected()
{
    // Identify disconnected client
    QTcpSocket *client =
        qobject_cast<QTcpSocket*>(sender());
    if (!client)
        return;

    // Remove client from list and clean up
    m_clients.removeAll(client);
    client->deleteLater();
}

