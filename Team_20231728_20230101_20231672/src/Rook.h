#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

/**
 * @class Rook
 * @brief Represents a Rook chess piece
 * 
 * The rook moves horizontally or vertically any number of squares.
 * It cannot jump over other pieces.
 */
class Rook : public Piece {
public:
    /**
     * @brief Constructor for Rook
     * @param color Color of the rook (WHITE or BLACK)
     * @param pos Initial position on the board
     */
    Rook(Color color, Position pos);
    
    /**
     * @brief Check if a move is valid for a rook
     * @param to Destination position
     * @param board Reference to the chess board
     * @return true if the move is horizontal/vertical and path is clear
     */
    bool isValidMove(Position to, ChessBoard& board) const override;
    
    /**
     * @brief Get the type of the piece as string
     * @return "Rook"
     */
    std::string getType() const override { return "Rook"; }
    
    /**
     * @brief Create a copy of this rook
     * @return Pointer to new Rook object
     */
    Rook* clone() const override;
};

#endif // ROOK_H