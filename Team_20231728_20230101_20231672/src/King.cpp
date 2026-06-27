#include "King.h"
#include "ChessBoard.h"
#include <cmath>  // For abs()

/**
 * @file King.cpp
 * @author Chess Game Developer
 * @date 2026
 * @brief Implementation of King class
 * @course Object Oriented Programming - Chess Game Project
 */

King::King(Color color, Position pos) 
    : Piece(color, pos, (color == Color::WHITE) ? 'K' : 'k') {}

bool King::isValidMove(Position to, ChessBoard& board) const {
    // Check if destination is within board
    if (!isPositionValid(to)) {
        return false;
    }
    
    // Calculate row and column differences
    int rowDiff = abs(to.row - position.row);
    int colDiff = abs(to.col - position.col);
    
    // King moves exactly one square in any direction
    if (rowDiff > 1 || colDiff > 1) {
        return false;
    }
    
    // Check if destination has a piece of the same color
    Piece* targetPiece = board.getPieceAt(to.row, to.col);
    if (targetPiece != nullptr && targetPiece->getColor() == pieceColor) {
        return false;  // Can't move to square occupied by own piece
    }
    
    // Simplified: Don't check if move puts king in check
    // (We'll handle this in ChessBoard class)
    
    return true;
}

King* King::clone() const {
    return new King(*this);  // Use copy constructor
}