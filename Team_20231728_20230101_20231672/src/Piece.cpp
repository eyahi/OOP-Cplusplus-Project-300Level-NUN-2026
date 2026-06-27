#include "Piece.h"
#include "ChessBoard.h"  // Forward declaration resolved

/**
 * @file Piece.cpp
 * @author Chess Game Developer
 * @date 2026
 * @brief Implementation of Piece base class
 * @course Object Oriented Programming - Chess Game Project
 */

/**
 * @brief Check if the path between two positions is clear of other pieces
 * 
 * This function works for rook, bishop, and queen movements.
 * It checks each square along the path (excluding start and end).
 * 
 * @param from Starting position
 * @param to Ending position
 * @param board Reference to the chess board
 * @return true if all squares between from and to are empty
 */
bool Piece::isPathClear(Position from, Position to, ChessBoard& board) const {
    // Calculate direction of movement
    int rowStep = 0;
    int colStep = 0;
    
    if (to.row > from.row) rowStep = 1;
    else if (to.row < from.row) rowStep = -1;
    
    if (to.col > from.col) colStep = 1;
    else if (to.col < from.col) colStep = -1;
    
    // Check each square along the path
    Position current = from;
    current.row += rowStep;
    current.col += colStep;
    
    while (!(current == to)) {
        // If any square along the path has a piece, path is not clear
        if (board.getPieceAt(current.row, current.col) != nullptr) {
            return false;
        }
        current.row += rowStep;
        current.col += colStep;
    }
    
    return true;
}