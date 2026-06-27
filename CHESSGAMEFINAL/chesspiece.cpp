#include "chesspiece.h"

ChessPiece::ChessPiece() : m_type(EMPTY), m_color(NONE), m_hasMoved(false)
{
}

ChessPiece::ChessPiece(PieceType type, PieceColor color)
    : m_type(type), m_color(color), m_hasMoved(false)
{
}

QString ChessPiece::getSymbol() const
{
    if (m_type == EMPTY) return "";

    // Unicode chess symbols
    if (m_color == WHITE) {
        switch (m_type) {
        case KING: return "♔";
        case QUEEN: return "♕";
        case ROOK: return "♖";
        case BISHOP: return "♗";
        case KNIGHT: return "♘";
        case PAWN: return "♙";
        default: return "";
        }
    } else {
        switch (m_type) {
        case KING: return "♚";
        case QUEEN: return "♛";
        case ROOK: return "♜";
        case BISHOP: return "♝";
        case KNIGHT: return "♞";
        case PAWN: return "♟";
        default: return "";
        }
    }
}
