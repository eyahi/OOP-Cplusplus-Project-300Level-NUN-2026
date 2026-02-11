/*
 * File: main.cpp
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Implementation for this component.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
