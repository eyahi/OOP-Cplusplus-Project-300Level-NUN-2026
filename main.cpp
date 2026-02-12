/*
  Names: Amira Dahiru Shettima, Roufiat ize saliu, rahma Sule habu
  course: object oriented programming
  filename: main.cpp
 Description: program entry point for the to do manager
 date: january 2026
*/
#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
