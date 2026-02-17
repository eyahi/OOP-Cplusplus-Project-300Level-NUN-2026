#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "chessboard.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void newGame();
    void showAbout();

private:
    void setupUI();
    void createMenus();

    ChessBoard *m_chessBoard;
};

#endif // MAINWINDOW_H
