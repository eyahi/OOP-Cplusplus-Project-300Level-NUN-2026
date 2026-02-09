#include "Rook.h"
#include "ChessBoard.h"
#include <cmath>  // For abs()

/**
 * @file Rook.cpp
 * @author Chess Game Developer
 * @date 2026
 * @brief Implementation of Rook class
 * @course Object Oriented Programming - Chess Game Project
 */

Rook::Rook(Color color, Position pos) 
    : Piece(color, pos, (color == Color::WHITE) ? 'R' : 'r') {}

bool Rook::isValidMove(Position to, ChessBoard& board) const {
    // Check if destination is within board
    if (!isPositionValid(to)) {
        return false;
    }
    
    // Calculate absolute differences
    int rowDiff = abs(to.row - position.row);
    int colDiff = abs(to.col - position.col);
    
    // Rook must move either horizontally OR vertically (not both)
    if (!(rowDiff == 0 && colDiff > 0) &&  // Horizontal move
        !(colDiff == 0 && rowDiff > 0)) {   // Vertical move
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

Rook* Rook::clone() const {
    return new Rook(*this);  // Use copy constructor
}