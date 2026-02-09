#ifndef BISHOP_H
#define BISHOP_H

#include "Piece.h"

/**
 * @class Bishop
 * @brief Represents a Bishop chess piece
 * 
 * The bishop moves diagonally any number of squares.
 * It cannot jump over other pieces.
 */
class Bishop : public Piece {
public:
    /**
     * @brief Constructor for Bishop
     * @param color Color of the bishop (WHITE or BLACK)
     * @param pos Initial position on the board
     */
    Bishop(Color color, Position pos);
    
    /**
     * @brief Check if a move is valid for a bishop
     * @param to Destination position
     * @param board Reference to the chess board
     * @return true if the move is diagonal and path is clear
     */
    bool isValidMove(Position to, ChessBoard& board) const override;
    
    /**
     * @brief Get the type of the piece as string
     * @return "Bishop"
     */
    std::string getType() const override { return "Bishop"; }
    
    /**
     * @brief Create a copy of this bishop
     * @return Pointer to new Bishop object
     */
    Bishop* clone() const override;
};

#endif // BISHOP_H