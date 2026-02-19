#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>

/*
 * ChatServer handles incoming client connections
 * and forwards messages between connected clients.
 *
 * OOP concepts used:
 * - Inheritance: ChatServer extends QTcpServer.
 * - Polymorphism: overrides incomingConnection().
 * - Encapsulation: client management is handled internally.
 */
class ChatServer : public QTcpServer
{
    Q_OBJECT

public:
    explicit ChatServer(QObject *parent = nullptr);

protected:
    // Overridden function called automatically when a client connects
    void incomingConnection(qintptr socketDescriptor) override;

private slots:
    // Reads messages sent by clients
    void onReadyRead();

    // Handles client disconnection
    void onDisconnected();

private:
    // Stores all currently connected clients
    QVector<QTcpSocket*> m_clients;
};

#endif // CHATSERVER_H

