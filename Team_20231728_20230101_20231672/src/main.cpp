#include "mainwindow.h"
#include "homescreen.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    HomeScreen home;
    MainWindow gameWindow;
    gameWindow.hide();

    // Connect home screen to show game
    QObject::connect(&home, &HomeScreen::playerVsPlayerSelected, [&]() {
        gameWindow.setGameMode(0);
        gameWindow.showMaximized();
    });

    QObject::connect(&home, &HomeScreen::playerVsAISelected, [&]() {
        gameWindow.setGameMode(1);
        gameWindow.showMaximized();
    });

    QObject::connect(&home, &HomeScreen::exitSelected, &a, &QApplication::quit);

    // Connect game window's return to menu
    QObject::connect(&gameWindow, &MainWindow::returnToMenuRequested, [&]() {
        gameWindow.hide(); // Hide game
        home.show(); // Show home again
    });

    // First show home
    home.show();

    return a.exec();
}
