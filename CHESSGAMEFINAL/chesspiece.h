#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QString>

enum PieceType {
    EMPTY = 0,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
};

enum PieceColor {
    NONE = 0,
    WHITE,
    BLACK
};

class ChessPiece
{
public:
    ChessPiece();
    ChessPiece(PieceType type, PieceColor color);

    PieceType getType() const { return m_type; }
    PieceColor getColor() const { return m_color; }
    QString getSymbol() const;
    bool isEmpty() const { return m_type == EMPTY; }
    bool hasMoved() const { return m_hasMoved; }
    void setMoved(bool moved) { m_hasMoved = moved; }

private:
    PieceType m_type;
    PieceColor m_color;
    bool m_hasMoved;
};

#endif // CHESSPIECE_H
