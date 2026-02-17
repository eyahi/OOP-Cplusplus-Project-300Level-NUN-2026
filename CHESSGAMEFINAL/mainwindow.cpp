#include "mainwindow.h"
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    createMenus();

    setWindowTitle("Professional Chess Game");
    setFixedSize(580, 630);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Title
    QLabel *titleLabel = new QLabel("Chess", centralWidget);
    QFont titleFont;
    titleFont.setPointSize(16);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    // Chess board
    m_chessBoard = new ChessBoard(centralWidget);
    layout->addWidget(m_chessBoard, 0, Qt::AlignCenter);

    layout->setContentsMargins(10, 10, 10, 10);
    setCentralWidget(centralWidget);
}

void MainWindow::createMenus()
{
    QMenuBar *menuBar = new QMenuBar(this);

    // Game menu
    QMenu *gameMenu = menuBar->addMenu("Game");
    QAction *newGameAction = gameMenu->addAction("New Game");
    connect(newGameAction, &QAction::triggered, this, &MainWindow::newGame);

    gameMenu->addSeparator();
    QAction *exitAction = gameMenu->addAction("Exit");
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    // Help menu
    QMenu *helpMenu = menuBar->addMenu("Help");
    QAction *aboutAction = helpMenu->addAction("About");
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAbout);

    setMenuBar(menuBar);
}

void MainWindow::newGame()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "New Game",
                                  "Start a new game? Current progress will be lost.",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        // Recreate the chess board
        QWidget *central = centralWidget();
        QVBoxLayout *layout = qobject_cast<QVBoxLayout*>(central->layout());

        layout->removeWidget(m_chessBoard);
        delete m_chessBoard;

        m_chessBoard = new ChessBoard(central);
        layout->addWidget(m_chessBoard, 0, Qt::AlignCenter);
    }
}

void MainWindow::showAbout()
{
    QMessageBox::about(this, "About Chess",
                       "<h2>Professional Chess Game</h2>"
                       "<p>A complete chess implementation with:</p>"
                       "<ul>"
                       "<li>Full chess rules</li>"
                       "<li>Castling and en passant</li>"
                       "<li>Pawn promotion</li>"
                       "<li>Check and checkmate detection</li>"
                       "</ul>"
                       "<p>Click on a piece to select it, then click on a highlighted square to move.</p>");
}
