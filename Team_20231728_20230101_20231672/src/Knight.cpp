#include "Knight.h"
#include "ChessBoard.h"
#include <cmath>  // For abs()

/**
 * @file Knight.cpp
 * @author Chess Game Developer
 * @date 2026
 * @brief Implementation of Knight class
 * @course Object Oriented Programming - Chess Game Project
 */

Knight::Knight(Color color, Position pos) 
    : Piece(color, pos, (color == Color::WHITE) ? 'N' : 'n') {}

bool Knight::isValidMove(Position to, ChessBoard& board) const {
    // Check if destination is within board
    if (!isPositionValid(to)) {
        return false;
    }
    
    // Calculate row and column differences
    int rowDiff = abs(to.row - position.row);
    int colDiff = abs(to.col - position.col);
    
    // Knight moves in L-shape: (2,1) or (1,2)
    bool isValidLShape = (rowDiff == 2 && colDiff == 1) || 
                         (rowDiff == 1 && colDiff == 2);
    
    if (!isValidLShape) {
        return false;
    }
    
    // Check if destination has a piece of the same color
    Piece* targetPiece = board.getPieceAt(to.row, to.col);
    if (targetPiece != nullptr && targetPiece->getColor() == pieceColor) {
        return false;  // Can't capture own piece
    }
    
    // Knight can jump over pieces, so no need to check path
    return true;
}

Knight* Knight::clone() const {
    return new Knight(*this);  // Use copy constructor
}