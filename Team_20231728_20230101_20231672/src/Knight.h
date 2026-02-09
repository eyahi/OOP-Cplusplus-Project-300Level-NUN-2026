#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

/**
 * @class Knight
 * @brief Represents a Knight chess piece
 * 
 * The knight moves in an L-shape: two squares in one direction and 
 * one square perpendicular, or one square in one direction and 
 * two squares perpendicular. The knight is the only piece that can "jump" over other pieces.
 */
class Knight : public Piece {
public:
    /**
     * @brief Constructor for Knight
     * @param color Color of the knight (WHITE or BLACK)
     * @param pos Initial position on the board
     */
    Knight(Color color, Position pos);
    
    /**
     * @brief Check if a move is valid for a knight
     * @param to Destination position
     * @param board Reference to the chess board (unused for knight's jump)
     * @return true if the move follows the knight's L-shaped pattern
     */
    bool isValidMove(Position to, ChessBoard& board) const override;
    
    /**
     * @brief Get the type of the piece as string
     * @return "Knight"
     */
    std::string getType() const override { return "Knight"; }
    
    /**
     * @brief Create a copy of this knight
     * @return Pointer to new Knight object
     */
    Knight* clone() const override;
};

#endif // KNIGHT_H