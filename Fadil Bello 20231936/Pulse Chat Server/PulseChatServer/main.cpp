#include <QCoreApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QMap>

QMap<QString, QTcpSocket*> clients; // username -> socket

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTcpServer server;

    if(!server.listen(QHostAddress::Any, 1234))
    {
        qDebug() << "Server failed to start!";
        return 1;
    }

    qDebug() << "Server started on port 1234";

    QObject::connect(&server, &QTcpServer::newConnection, [&]()
                     {
                         QTcpSocket *client = server.nextPendingConnection();
                         qDebug() << "New connection";

                         // READ DATA
                         QObject::connect(client, &QTcpSocket::readyRead, [client]()
                                          {
                                              QByteArray data = client->readAll();
                                              QString msg = QString::fromUtf8(data).trimmed();

                                              // LOGIN
                                              if(msg.startsWith("LOGIN:"))
                                              {
                                                  QString username = msg.mid(6).trimmed();
                                                  clients[username] = client;

                                                  qDebug() << username << "joined";

                                                  // send existing users to new client
                                                  for(auto existing : clients.keys())
                                                  {
                                                      if(existing != username)
                                                          client->write(("JOIN:" + existing + "\n").toUtf8());
                                                  }

                                                  // notify everyone about new user
                                                  for(auto c : clients.values())
                                                      c->write(("JOIN:" + username + "\n").toUtf8());

                                                  return;
                                              }

                                              // PRIVATE MESSAGE
                                              if(msg.startsWith("TO:"))
                                              {
                                                  QString payload = msg.mid(3);

                                                  QString target  = payload.section("|",0,0);
                                                  QString idPart  = payload.section("|",1,1);
                                                  QString msgPart = payload.section("|",2,2);

                                                  QString msgId = idPart.mid(3);
                                                  QString text  = msgPart.mid(4);

                                                  QString sender = clients.key(client);

                                                  // user must exist
                                                  if(!clients.contains(target))
                                                      return;

                                                  QTcpSocket *receiver = clients[target];

                                                  // must be REALLY online
                                                  if(!receiver || receiver->state() != QAbstractSocket::ConnectedState)
                                                      return;

                                                  // send message
                                                  receiver->write(
                                                      ("FROM:" + sender +
                                                       "|ID:" + msgId +
                                                       "|MSG:" + text + "\n").toUtf8()
                                                      );

                                                  // confirm delivery
                                                  client->write(
                                                      ("DELIVERED:" + msgId + "\n").toUtf8()
                                                      );
                                              }
                                          });

                         // DISCONNECT
                         QObject::connect(client, &QTcpSocket::disconnected, [client]()
                                          {
                                              QString name = clients.key(client);
                                              clients.remove(name);

                                              qDebug() << name << "left";

                                              for(auto c : clients.values())
                                                  c->write(("LEFT:" + name + "\n").toUtf8());

                                              client->deleteLater();
                                          });
                     });

    return a.exec();
}
