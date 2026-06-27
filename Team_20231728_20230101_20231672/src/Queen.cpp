#include "Queen.h"
#include "ChessBoard.h"
#include <cmath>  // For abs()

/**
 * @file Queen.cpp
 * @author Chess Game Developer
 * @date 2026
 * @brief Implementation of Queen class
 * @course Object Oriented Programming - Chess Game Project
 */

Queen::Queen(Color color, Position pos) 
    : Piece(color, pos, (color == Color::WHITE) ? 'Q' : 'q') {}

bool Queen::isValidMove(Position to, ChessBoard& board) const {
    // Check if destination is within board
    if (!isPositionValid(to)) {
        return false;
    }
    
    // Calculate absolute differences
    int rowDiff = abs(to.row - position.row);
    int colDiff = abs(to.col - position.col);
    
    // Queen moves like a rook OR a bishop
    bool isRookMove = (rowDiff == 0 && colDiff > 0) ||  // Horizontal
                      (colDiff == 0 && rowDiff > 0);    // Vertical
    
    bool isBishopMove = (rowDiff == colDiff && rowDiff > 0);  // Diagonal
    
    if (!isRookMove && !isBishopMove) {
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

Queen* Queen::clone() const {
    return new Queen(*this);  // Use copy constructor
}