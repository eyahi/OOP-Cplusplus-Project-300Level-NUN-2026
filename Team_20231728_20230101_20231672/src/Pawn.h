#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

/**
 * @class Pawn
 * @brief Represents a Pawn chess piece
 * 
 * The pawn moves forward one square, but captures diagonally.
 * On its first move, it can move forward two squares.
 * This is a simplified version without en passant or promotion.
 */
class Pawn : public Piece {
public:
    /**
     * @brief Constructor for Pawn
     * @param color Color of the pawn (WHITE or BLACK)
     * @param pos Initial position on the board
     */
    Pawn(Color color, Position pos);
    
    /**
     * @brief Check if a move is valid for a pawn
     * @param to Destination position
     * @param board Reference to the chess board
     * @return true if the move is valid according to pawn rules
     */
    bool isValidMove(Position to, ChessBoard& board) const override;
    
    /**
     * @brief Get the type of the piece as string
     * @return "Pawn"
     */
    std::string getType() const override { return "Pawn"; }
    
    /**
     * @brief Create a copy of this pawn
     * @return Pointer to new Pawn object
     */
    Pawn* clone() const override;
    
private:
    /**
     * @brief Get the forward direction based on pawn color
     * @return 1 for BLACK (down), -1 for WHITE (up)
     */
    int getForwardDirection() const {
        return (pieceColor == Color::BLACK) ? 1 : -1;
    }
};

#endif // PAWN_H