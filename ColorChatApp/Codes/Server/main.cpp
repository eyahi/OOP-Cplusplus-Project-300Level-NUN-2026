/*
 * File: main.cpp
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Implementation for this component.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#include <QCoreApplication>
#include "ChatServer.h"

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);

    ChatServer server(8080);

    if (!server.start()) {
        return 1;
    }

    qDebug() << "Server ready. Press Ctrl+C to stop.";
    return app.exec();
}