#include "Bishop.h"
#include "ChessBoard.h"
#include <cmath>  // For abs()

/**
 * @file Bishop.cpp
 * @author Chess Game Developer
 * @date 2026
 * @brief Implementation of Bishop class
 * @course Object Oriented Programming - Chess Game Project
 */

Bishop::Bishop(Color color, Position pos) 
    : Piece(color, pos, (color == Color::WHITE) ? 'B' : 'b') {}

bool Bishop::isValidMove(Position to, ChessBoard& board) const {
    // Check if destination is within board
    if (!isPositionValid(to)) {
        return false;
    }
    
    // Calculate absolute differences
    int rowDiff = abs(to.row - position.row);
    int colDiff = abs(to.col - position.col);
    
    // Bishop must move diagonally (equal row and col changes)
    if (rowDiff != colDiff) {
        return false;
    }
    
    // Can't stay in the same place
    if (rowDiff == 0) {
        return false;
    }
    
    // Check if path is clear
    if (!isPathClear(position, to, board)) {
        return false;
    }
    
    // Check if destination has a piece of the same color
    Piece* targetPiece = board.getPieceAt(to.row, to.col);
    if (targetPiece != nullptr && targetPiece->getColor() == pieceColor) {
        return false;  // Can't capture own piece
    }
    
    return true;
}

Bishop* Bishop::clone() const {
    return new Bishop(*this);  // Use copy constructor
}