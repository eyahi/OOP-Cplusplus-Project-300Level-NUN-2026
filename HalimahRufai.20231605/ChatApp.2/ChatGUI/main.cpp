#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "messagemodel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    // Create the QML engine
    QQmlApplicationEngine engine;

    // Create your message model and expose it to QML
    MessageModel msgModel;
    engine.rootContext()->setContextProperty("msgModel", &msgModel);

    // Absolute path to your Main.qml
    const QString qmlPath =
        "C:/Users/HALIMAH/Videos/ChatApp.2/ChatGUI/Main.qml";

    // Load QML
    engine.load(QUrl::fromLocalFile(qmlPath));

    // Check if loading succeeded
    if (engine.rootObjects().isEmpty()) {
        qWarning("Failed to load QML file!");
        return -1;
    }

    return app.exec();
}
