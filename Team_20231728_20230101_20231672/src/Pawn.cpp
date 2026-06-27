#include "Pawn.h"
#include "ChessBoard.h"
#include <cmath>  // For abs()

/**
 * @file Pawn.cpp
 * @author Chess Game Developer
 * @date 2026
 * @brief Implementation of Pawn class
 * @course Object Oriented Programming - Chess Game Project
 */

Pawn::Pawn(Color color, Position pos) 
    : Piece(color, pos, (color == Color::WHITE) ? 'P' : 'p') {}

bool Pawn::isValidMove(Position to, ChessBoard& board) const {
    // Check if destination is within board
    if (!isPositionValid(to)) {
        return false;
    }
    
    int forwardDir = getForwardDirection();
    
    // Check for normal forward move (one square)
    if (to.col == position.col && 
        to.row == position.row + forwardDir &&
        board.getPieceAt(to.row, to.col) == nullptr) {
        return true;
    }
    
    // Check for initial two-square move
    if (!hasMoved && 
        to.col == position.col && 
        to.row == position.row + (2 * forwardDir) &&
        board.getPieceAt(to.row, to.col) == nullptr &&
        // Check if the square in between is empty
        board.getPieceAt(position.row + forwardDir, position.col) == nullptr) {
        return true;
    }
    
    // Check for diagonal capture
    if (abs(to.col - position.col) == 1 &&
        to.row == position.row + forwardDir) {
        Piece* targetPiece = board.getPieceAt(to.row, to.col);
        // Can capture if there's an opponent piece at destination
        if (targetPiece != nullptr && targetPiece->getColor() != pieceColor) {
            return true;
        }
    }
    
    return false;
}

Pawn* Pawn::clone() const {
    return new Pawn(*this);  // Use copy constructor
}