#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QVector>
#include <QPair>
#include "chesspiece.h"

struct Position {
    int row;
    int col;
    Position(int r = -1, int c = -1) : row(r), col(c) {}
    bool isValid() const { return row >= 0 && row < 8 && col >= 0 && col < 8; }
    bool operator==(const Position& other) const { return row == other.row && col == other.col; }
};

class ChessBoard : public QWidget
{
    Q_OBJECT

public:
    explicit ChessBoard(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    void initializeBoard();
    void drawBoard(QPainter &painter);
    void drawPieces(QPainter &painter);

    bool isValidMove(const Position &from, const Position &to);
    QVector<Position> getValidMoves(const Position &pos);
    bool wouldBeInCheck(const Position &from, const Position &to, PieceColor color);
    bool isInCheck(PieceColor color);
    bool isCheckmate(PieceColor color);
    bool isStalemate(PieceColor color);

    void makeMove(const Position &from, const Position &to);
    void handlePromotion(const Position &pos);

    Position getBoardPosition(int x, int y);

    ChessPiece m_board[8][8];
    Position m_selectedPos;
    QVector<Position> m_validMoves;
    PieceColor m_currentPlayer;
    Position m_enPassantTarget;

    static const int SQUARE_SIZE = 70;
};

#endif // CHESSBOARD_H
