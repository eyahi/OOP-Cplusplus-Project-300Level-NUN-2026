#include "loginwindow.h"
#include <QApplication>
#include <QCoreApplication>

// Application entry point.
int main(int argc, char *argv[])
{
    // Qt application instance.
    QApplication a(argc, argv);
    // App identity (used for AppData paths).
    QCoreApplication::setOrganizationName("SupermarketInventory");
    QCoreApplication::setApplicationName("SupermarketInventory");

    // Show the login window.
    LoginWindow w;
    w.show();

    // Start the Qt event loop.
    return a.exec();
}
