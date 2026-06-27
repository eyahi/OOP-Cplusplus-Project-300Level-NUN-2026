#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ChessBoard.h"
#include "chesswidget.h"
#include <QLabel>
#include <QPushButton>
#include "pausedialog.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setGameMode(int mode); // 0 = PvP, 1 = PvAI

private:
    Ui::MainWindow *ui;
    ChessBoard* chessBoard;
    ChessWidget* chessWidget;
    int gameMode; // 0 = PvP, 1 = PvAI

    // UI elements
    QLabel* player1Label;
    QLabel* player2Label;
    QPushButton* pauseButton;
    QPushButton* undoButton;
    QLabel* statusLabel;

    QTimer* aiTimer;

private slots:
    void onPauseButtonClicked();
    void onResumeClicked();
    void onRestartClicked();
    void onMenuClicked();
    void onQuitClicked();
    void updateGameStatus();
    void onUndoButtonClicked();
    void makeAIMove();

signals:
    void returnToMenuRequested();
};

#endif // MAINWINDOW_H
