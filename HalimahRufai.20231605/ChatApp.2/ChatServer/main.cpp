#include <QCoreApplication>
#include "chatserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    ChatServer server;

    if (!server.isListening())
        qWarning("Server failed to start!");
    else
        qInfo("Chat server running on port 1234");

    return app.exec();
}
