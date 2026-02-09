/*
 * Author: Itoro Ifon, jason Hippolite, Prince Umeh
 * Date: 2026-01-22
 * Course/Assignment: C++ Project - Qt Music Player
 * File: main.cpp
 * Purpose: Application entry point. Creates the Qt application object and
 *          launches the main window.
 */
#include <QApplication>
#include "mainwindow.h"
/*
 * Function: main
 * Purpose: Creates the QApplication instance and displays the main window.
 * Parameters:
 *   - argc: number of command-line arguments
 *   - argv: array of command-line argument strings
 * Returns:
 *   - int: application exit code (0 means successful exit)
 */

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    w.setWindowTitle("Qt Music Player");
    w.show();
    return app.exec();
}
