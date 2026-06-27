#ifndef KING_H
#define KING_H

#include "Piece.h"

/**
 * @class King
 * @brief Represents a King chess piece
 * 
 * The king moves one square in any direction (horizontal, vertical, or diagonal).
 * This is a simplified version without castling.
 */
class King : public Piece {
public:
    /**
     * @brief Constructor for King
     * @param color Color of the king (WHITE or BLACK)
     * @param pos Initial position on the board
     */
    King(Color color, Position pos);
    
    /**
     * @brief Check if a move is valid for a king
     * @param to Destination position
     * @param board Reference to the chess board
     * @return true if the move is one square in any direction
     */
    bool isValidMove(Position to, ChessBoard& board) const override;
    
    /**
     * @brief Get the type of the piece as string
     * @return "King"
     */
    std::string getType() const override { return "King"; }
    
    /**
     * @brief Create a copy of this king
     * @return Pointer to new King object
     */
    King* clone() const override;
};

#endif // KING_H