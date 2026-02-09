#include "chesswidget.h"
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QMouseEvent>
#include "ChessBoard.h"
#include "King.h"
#include "Queen.h"
#include "Rook.h"
#include "Bishop.h"
#include "Knight.h"
#include "Pawn.h"

ChessWidget::ChessWidget(QWidget *parent) : QWidget(parent), chessBoard(nullptr)
{
    // Size will be determined by parent widget
    // We'll calculate square size dynamically in paintEvent
    selectedPiece = Position(-1, -1);  // No selection initially
    loadPieceImages();
}

void ChessWidget::setChessBoard(ChessBoard* board)
{
    chessBoard = board;
    update(); // Trigger repaint
}

QString ChessWidget::getImageName(Piece* piece) const
{
    if (!piece) return ""; // Empty square

    QString color = (piece->getColor() == Color::WHITE) ? "w" : "b";
    std::string type = piece->getType(); // e.g., "King", "Queen"

    // Convert to your filename abbreviations
    if (type == "King") return color + "k";
    if (type == "Queen") return color + "q";
    if (type == "Rook") return color + "r";
    if (type == "Bishop") return color + "b";
    if (type == "Knight") return color + "n";
    if (type == "Pawn") return color + "p";

    return "";
}

void ChessWidget::loadPieceImages()
{
    QString pieces[] = {"wk", "wq", "wr", "wb", "wn", "wp",
                        "bk", "bq", "br", "bb", "bn", "bp"};

    QString basePath = "C:/Users/albaq/Documents/Chess/images/";

    for (const QString& piece : pieces) {
        QString filePath = basePath + piece + ".png";
        pieceImages[piece] = QPixmap(filePath);

        if (pieceImages[piece].isNull()) {
            qDebug() << "Failed to load:" << filePath;
        } else {
            qDebug() << "Loaded:" << filePath;
        }
    }
}

void ChessWidget::mousePressEvent(QMouseEvent* event)
{
    int boardWidth = width();
    int boardHeight = height();
    int squareSize = qMin(boardWidth, boardHeight) / 8;
    int xOffset = (boardWidth - (squareSize * 8)) / 2;
    int yOffset = (boardHeight - (squareSize * 8)) / 2;

    int col = (event->position().x() - xOffset) / squareSize;
    int row = (event->position().y() - yOffset) / squareSize;

    if (row >= 0 && row < 8 && col >= 0 && col < 8) {
        Position clickedPos(row, col);

        // If we have a selected piece, try to move it
        if (selectedPiece.isValid()) {
            // Try to make the move
            if (chessBoard && chessBoard->makeMove(selectedPiece, clickedPos)) {
                qDebug() << "Move successful from (" << selectedPiece.row << "," << selectedPiece.col
                         << ") to (" << row << "," << col << ")";
                // Clear selection
                selectedPiece = Position(-1, -1);
                possibleMoves.clear();

                emit boardStateChanged(); // <-- ADDED

            } else {
                // Invalid move or clicked on another piece - select new piece
                selectedPiece = clickedPos;
                // TODO: Calculate possible moves for this piece
                possibleMoves.clear();
            }
        } else {
            // No piece selected - select this one if it has a piece
            Piece* piece = chessBoard ? chessBoard->getPieceAt(row, col) : nullptr;
            if (piece && piece->getColor() == chessBoard->getCurrentPlayer()) {
                selectedPiece = clickedPos;
                // TODO: Calculate possible moves for this piece
                possibleMoves.clear();
                qDebug() << "Selected piece at (" << row << "," << col << ")";
            }
        }

        update(); // Redraw board
        emit squareClicked(row, col);
    }
}

void ChessWidget::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);

    // Calculate dynamic square size
    int boardWidth = width();
    int boardHeight = height();
    int squareSize = qMin(boardWidth, boardHeight) / 8;

    // Center the board
    int xOffset = (boardWidth - (squareSize * 8)) / 2;
    int yOffset = (boardHeight - (squareSize * 8)) / 2;

    // Draw 8×8 grid
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            QColor color = ((row + col) % 2 == 0) ? QColor(240, 217, 181) : QColor(181, 136, 99);
            painter.fillRect(xOffset + col * squareSize,
                             yOffset + row * squareSize,
                             squareSize, squareSize, color);
        }
    }

    // Highlight selected square
    if (selectedPiece.isValid()) {
        painter.setBrush(QColor(255, 255, 0, 100)); // Semi-transparent yellow
        painter.setPen(Qt::NoPen);
        painter.drawRect(xOffset + selectedPiece.col * squareSize,
                         yOffset + selectedPiece.row * squareSize,
                         squareSize, squareSize);
    }

    // Highlight possible moves
    painter.setBrush(QColor(0, 255, 0, 100)); // Semi-transparent green
    painter.setPen(Qt::NoPen);
    for (const Position& move : possibleMoves) {
        painter.drawRect(xOffset + move.col * squareSize,
                         yOffset + move.row * squareSize,
                         squareSize, squareSize);
    }

    // Draw chess pieces if we have a board
    if (chessBoard) {
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                Piece* piece = chessBoard->getPieceAt(row, col);
                if (piece) {
                    QString imageKey = getImageName(piece);
                    if (pieceImages.contains(imageKey)) {
                        QPixmap scaledPiece = pieceImages[imageKey].scaled(squareSize, squareSize,
                                                                           Qt::KeepAspectRatio, Qt::SmoothTransformation);
                        painter.drawPixmap(xOffset + col * squareSize,
                                           yOffset + row * squareSize,
                                           scaledPiece);
                    }
                }
            }
        }
    }
}
void ChessWidget::clearSelection()
{
    selectedPiece = Position(-1, -1);
    possibleMoves.clear();
    update();
}
