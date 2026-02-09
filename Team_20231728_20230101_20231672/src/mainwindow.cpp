#include "mainwindow.h"
#include <QDebug>
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include "pausedialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , chessBoard(nullptr)
    , chessWidget(nullptr)
    , gameMode(0)
    , aiTimer(nullptr)
{
    ui->setupUi(this);

    // Create chess logic
    chessBoard = new ChessBoard();

    // Create visual widget
    chessWidget = new ChessWidget(this);
    chessWidget->setChessBoard(chessBoard);

    // Connect chess widget signals
    connect(chessWidget, &ChessWidget::boardStateChanged, this, &MainWindow::updateGameStatus);

    // Create AI timer
    aiTimer = new QTimer(this);
    aiTimer->setSingleShot(true);
    connect(aiTimer, &QTimer::timeout, this, &MainWindow::makeAIMove);

    // Create player 2 label (top left - faces black pieces)
    player2Label = new QLabel("Player 2 (Black)");
    player2Label->setAlignment(Qt::AlignLeft);
    player2Label->setStyleSheet("font-size: 16px; font-weight: bold; padding: 10px;");

    // Create buttons (top right)
    pauseButton = new QPushButton("⏸️ Pause");
    undoButton = new QPushButton("↩️ Undo");

    // Connect pause button
    connect(pauseButton, &QPushButton::clicked, this, &MainWindow::onPauseButtonClicked);

    // Connect undo button
    connect(undoButton, &QPushButton::clicked, this, &MainWindow::onUndoButtonClicked);

    // Top layout: Player 2 label + buttons
    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(player2Label);
    topLayout->addStretch();
    topLayout->addWidget(pauseButton);
    topLayout->addWidget(undoButton);

    // Create player 1 label (bottom left - faces white pieces)
    player1Label = new QLabel("Player 1 (White)");
    player1Label->setAlignment(Qt::AlignLeft);
    player1Label->setStyleSheet("font-size: 16px; font-weight: bold; padding: 10px;");

    // Bottom layout: Player 1 label
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(player1Label);
    bottomLayout->addStretch(); // Push to left

    // Status label (between chessboard and bottom layout)
    statusLabel = new QLabel("White's turn");
    statusLabel->setAlignment(Qt::AlignCenter);
    statusLabel->setStyleSheet("font-size: 16px; font-weight: bold; padding: 8px; color: white; background-color: #333; border-radius: 5px;");

    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(chessWidget, 1); // Chessboard expands
    mainLayout->addWidget(statusLabel);    // Status display
    mainLayout->addLayout(bottomLayout);

    // Set central widget
    QWidget* centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Initial status update
    updateGameStatus();

    qDebug() << "ChessBoard created at:" << chessBoard;
    qDebug() << "ChessWidget created at:" << chessWidget;
}

void MainWindow::setGameMode(int mode)
{
    gameMode = mode;
    qDebug() << "Game mode set to:" << (mode == 0 ? "Player vs Player" : "Player vs AI");

    // Update label based on game mode
    if (mode == 0) {
        player2Label->setText("Player 2 (Black)");
    } else {
        player2Label->setText("AI Opponent (Black)");
    }
}

void MainWindow::updateGameStatus()
{
    if (!chessBoard) return;

    QString status;
    Color current = chessBoard->getCurrentPlayer();

    // Turn info
    status = (current == Color::WHITE) ? "White's turn" : "Black's turn";

    // Check/Checkmate
    if (chessBoard->isInCheck(current)) {
        if (chessBoard->isCheckmate(current)) {
            status = (current == Color::WHITE) ? "Checkmate! Black wins!" : "Checkmate! White wins!";
        } else {
            status += " - CHECK!";
        }
    } else if (chessBoard->isStalemate(current)) {
        status = "Stalemate! Game is a draw.";
    }

    statusLabel->setText(status);

    // Trigger AI move if it's AI's turn
    if (gameMode == 1 && chessBoard->getCurrentPlayer() == Color::BLACK) {
        aiTimer->start(500); // 0.5 second delay before AI moves
    }
}

void MainWindow::onPauseButtonClicked()
{
    PauseDialog pauseDialog(this);
    pauseDialog.setModal(true);

    connect(&pauseDialog, &PauseDialog::resumeClicked, this, &MainWindow::onResumeClicked);
    connect(&pauseDialog, &PauseDialog::restartClicked, this, &MainWindow::onRestartClicked);
    connect(&pauseDialog, &PauseDialog::menuClicked, this, &MainWindow::onMenuClicked);
    connect(&pauseDialog, &PauseDialog::quitClicked, this, &MainWindow::onQuitClicked);

    pauseDialog.exec();
}

void MainWindow::onResumeClicked()
{
    // Just close the dialog (already done)
    qDebug() << "Game resumed";
}

void MainWindow::onRestartClicked()
{
    qDebug() << "Restart game";
    if (chessBoard) {
        chessBoard->resetBoard();
        chessWidget->clearSelection();
        updateGameStatus();
    }
}

void MainWindow::onMenuClicked()
{
    qDebug() << "Return to menu";
    // Reset the board first
    if (chessBoard) {
        chessBoard->resetBoard();
        chessWidget->clearSelection();
    }
    emit returnToMenuRequested();
}

void MainWindow::onQuitClicked()
{
    qDebug() << "Quit game";
    QApplication::quit();
}

void MainWindow::onUndoButtonClicked()
{
    if (chessBoard && chessBoard->undoLastMove()) {
        chessWidget->clearSelection();
        updateGameStatus();
        qDebug() << "Move undone";
    } else {
        qDebug() << "No moves to undo";
    }
}

void MainWindow::makeAIMove()
{
    if (!chessBoard || gameMode != 1) return;

    // Make sure it's still AI's turn (black)
    if (chessBoard->getCurrentPlayer() == Color::BLACK) {
        Move aiMove = chessBoard->getRandomMove(Color::BLACK);
        if (aiMove.from.isValid() && aiMove.to.isValid()) {
            if (chessBoard->makeMove(aiMove.from, aiMove.to)) {
                chessWidget->update();
                updateGameStatus(); // This will update status and potentially trigger next AI move
                qDebug() << "AI moved from (" << aiMove.from.row << "," << aiMove.from.col
                         << ") to (" << aiMove.to.row << "," << aiMove.to.col << ")";
            }
        }
    }
}

MainWindow::~MainWindow()
{
    delete aiTimer;
    delete chessBoard;
    delete chessWidget;
    delete ui;
}
