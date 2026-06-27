#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

/**
 * @class Queen
 * @brief Represents a Queen chess piece
 * 
 * The queen moves horizontally, vertically, or diagonally any number of squares.
 * It combines the moves of a rook and bishop.
 * It cannot jump over other pieces.
 */
class Queen : public Piece {
public:
    /**
     * @brief Constructor for Queen
     * @param color Color of the queen (WHITE or BLACK)
     * @param pos Initial position on the board
     */
    Queen(Color color, Position pos);
    
    /**
     * @brief Check if a move is valid for a queen
     * @param to Destination position
     * @param board Reference to the chess board
     * @return true if the move is horizontal, vertical, or diagonal with clear path
     */
    bool isValidMove(Position to, ChessBoard& board) const override;
    
    /**
     * @brief Get the type of the piece as string
     * @return "Queen"
     */
    std::string getType() const override { return "Queen"; }
    
    /**
     * @brief Create a copy of this queen
     * @return Pointer to new Queen object
     */
    Queen* clone() const override;
};

#endif // QUEEN_H