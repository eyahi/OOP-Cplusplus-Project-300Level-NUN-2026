#include "ChessBoard.h"
#include "Pawn.h"
#include "Knight.h"
#include "Bishop.h"
#include "Rook.h"
#include "Queen.h"
#include "King.h"
#include <iostream>
#include <cstdlib>  // For rand()
#include <ctime>    // For time()

/**
 * @file ChessBoard.cpp
 * @author Student Name
 * @date 2024
 * @brief Implementation of ChessBoard class - Core game logic
 * @course Object Oriented Programming - Chess Game Project
 */

// Helper function to check if position is valid
bool isValidPosition(int row, int col) {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

// Constructor - Initialize new chess board
ChessBoard::ChessBoard() : currentPlayer(Color::WHITE) {
    // Seed random number generator for AI
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    
    // Initialize all board positions to nullptr
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            board[row][col] = nullptr;
        }
    }
    
    // Set up initial board position
    initializeBoard();
}

// Destructor - Clean up all pieces
ChessBoard::~ChessBoard() {
    clearBoard();
}

// Copy constructor for board copying
ChessBoard::ChessBoard(const ChessBoard& other) : currentPlayer(other.currentPlayer) {
    // Initialize empty board
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            board[row][col] = nullptr;
        }
    }
    
    // Deep copy all pieces
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (other.board[row][col] != nullptr) {
                board[row][col] = other.board[row][col]->clone();
            }
        }
    }
    
    // Copy move history
    moveHistory = other.moveHistory;
}

// Assignment operator
ChessBoard& ChessBoard::operator=(const ChessBoard& other) {
    if (this != &other) {
        // Clear current board
        clearBoard();
        
        // Copy pieces
        for (int row = 0; row < 8; row++) {
            for (int col = 0; col < 8; col++) {
                if (other.board[row][col] != nullptr) {
                    board[row][col] = other.board[row][col]->clone();
                }
            }
        }
        
        currentPlayer = other.currentPlayer;
        moveHistory = other.moveHistory;
    }
    return *this;
}

// Initialize board with starting position
void ChessBoard::initializeBoard() {
    // Clear any existing pieces first
    clearBoard();
    
    // Place pawns
    for (int col = 0; col < 8; col++) {
        // White pawns on row 6
        board[6][col] = new Pawn(Color::WHITE, Position(6, col));
        // Black pawns on row 1
        board[1][col] = new Pawn(Color::BLACK, Position(1, col));
    }
    
    // Place rooks
    board[7][0] = new Rook(Color::WHITE, Position(7, 0));  // White rook a1
    board[7][7] = new Rook(Color::WHITE, Position(7, 7));  // White rook h1
    board[0][0] = new Rook(Color::BLACK, Position(0, 0));  // Black rook a8
    board[0][7] = new Rook(Color::BLACK, Position(0, 7));  // Black rook h8
    
    // Place knights
    board[7][1] = new Knight(Color::WHITE, Position(7, 1));  // White knight b1
    board[7][6] = new Knight(Color::WHITE, Position(7, 6));  // White knight g1
    board[0][1] = new Knight(Color::BLACK, Position(0, 1));  // Black knight b8
    board[0][6] = new Knight(Color::BLACK, Position(0, 6));  // Black knight g8
    
    // Place bishops
    board[7][2] = new Bishop(Color::WHITE, Position(7, 2));  // White bishop c1
    board[7][5] = new Bishop(Color::WHITE, Position(7, 5));  // White bishop f1
    board[0][2] = new Bishop(Color::BLACK, Position(0, 2));  // Black bishop c8
    board[0][5] = new Bishop(Color::BLACK, Position(0, 5));  // Black bishop f8
    
    // Place queens
    board[7][3] = new Queen(Color::WHITE, Position(7, 3));   // White queen d1
    board[0][3] = new Queen(Color::BLACK, Position(0, 3));   // Black queen d8
    
    // Place kings
    board[7][4] = new King(Color::WHITE, Position(7, 4));    // White king e1
    board[0][4] = new King(Color::BLACK, Position(0, 4));    // Black king e8
}

// Clear all pieces from board
void ChessBoard::clearBoard() {
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col] != nullptr) {
                delete board[row][col];
                board[row][col] = nullptr;
            }
        }
    }
}

// Get piece at specific position
Piece* ChessBoard::getPieceAt(int row, int col) const {
    if (!isValidPosition(row, col)) {
        return nullptr;
    }
    return board[row][col];
}

Piece* ChessBoard::getPieceAt(Position pos) const {
    return getPieceAt(pos.row, pos.col);
}

// Check if a square is under attack by pieces of a specific color
bool ChessBoard::isSquareUnderAttack(Position square, Color attackerColor) const {
    // Check all squares on the board
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece* piece = board[row][col];
            if (piece != nullptr && piece->getColor() == attackerColor) {
                // Check if this piece can move to the square
                Position from(row, col);
                
                // First check if the move is valid for the piece type
                if (piece->isValidMove(square, *const_cast<ChessBoard*>(this))) {
                    // Check if destination has same color piece
                    Piece* targetPiece = board[square.row][square.col];
                    if (targetPiece == nullptr || targetPiece->getColor() != attackerColor) {
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

// Check if a player is in check
bool ChessBoard::isInCheck(Color color) const {
    // Find the king of this color
    Position kingPos(-1, -1);
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            Piece* piece = board[row][col];
            if (piece != nullptr && 
                piece->getColor() == color && 
                piece->getType() == "King") {
                kingPos = Position(row, col);
                break;
            }
        }
        if (kingPos.isValid()) break;
    }
    
    if (!kingPos.isValid()) {
        return false;  // No king found (shouldn't happen in valid game)
    }
    
    // Check if king's square is under attack by opponent
    Color opponent = (color == Color::WHITE) ? Color::BLACK : Color::WHITE;
    return isSquareUnderAttack(kingPos, opponent);
}

// Check if a move is legal (considering check)
bool ChessBoard::isMoveLegal(Position from, Position to) const {
    // Basic validation
    if (!from.isValid() || !to.isValid()) {
        return false;
    }
    
    Piece* piece = getPieceAt(from);
    if (piece == nullptr || piece->getColor() != currentPlayer) {
        return false;  // No piece or wrong color
    }
    
    // Check if move is valid according to piece rules
    if (!piece->isValidMove(to, *const_cast<ChessBoard*>(this))) {
        return false;
    }
    
    // Check if destination has own piece
    Piece* targetPiece = getPieceAt(to);
    if (targetPiece != nullptr && targetPiece->getColor() == piece->getColor()) {
        return false;
    }
    
    // Simulate the move to check if it leaves king in check
    ChessBoard tempBoard(*this);  // Create a copy of the board
    
    // Temporarily make the move on the temp board
    Piece* tempPiece = tempBoard.board[from.row][from.col];
    Piece* tempTarget = tempBoard.board[to.row][to.col];
    
    // Move the piece
    tempBoard.board[to.row][to.col] = tempPiece;
    tempBoard.board[from.row][from.col] = nullptr;
    if (tempPiece) {
        tempPiece->setPosition(to);
    }
    
    // Check if the moving player would be in check after the move
    bool wouldBeInCheck = tempBoard.isInCheck(piece->getColor());
    
    // Restore the temporary board (not strictly necessary since it's a copy)
    tempBoard.board[from.row][from.col] = tempPiece;
    tempBoard.board[to.row][to.col] = tempTarget;
    if (tempPiece) {
        tempPiece->setPosition(from);
    }
    
    return !wouldBeInCheck;
}

// Execute a move (internal use - doesn't check legality)
void ChessBoard::executeMove(Position from, Position to) {
    Piece* piece = board[from.row][from.col];
    if (piece == nullptr) return;
    
    // Capture piece at destination if any
    Piece* capturedPiece = board[to.row][to.col];
    
    // Store move in history BEFORE moving (captured piece is still in place)
    moveHistory.push_back(Move(from, to, capturedPiece));
    
    // Move the piece
    board[to.row][to.col] = piece;
    board[from.row][from.col] = nullptr;
    piece->setPosition(to);
    
    // If we captured a piece, delete it
    if (capturedPiece != nullptr) {
        // Note: We don't delete here because we store pointer in moveHistory
        // Actual deletion happens in destructor
    }
    
    // Switch player
    currentPlayer = (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;
}

// Make a move (public interface)
bool ChessBoard::makeMove(Position from, Position to) {
    if (!isMoveLegal(from, to)) {
        return false;
    }
    
    executeMove(from, to);
    return true;
}

// Undo the last move
bool ChessBoard::undoLastMove() {
    if (moveHistory.empty()) {
        return false;
    }
    
    Move lastMove = moveHistory.back();
    moveHistory.pop_back();
    
    // Move piece back to original position
    Piece* movedPiece = board[lastMove.to.row][lastMove.to.col];
    board[lastMove.from.row][lastMove.from.col] = movedPiece;
    
    // Restore captured piece if any
    board[lastMove.to.row][lastMove.to.col] = lastMove.capturedPiece;
    
    if (movedPiece != nullptr) {
        movedPiece->setPosition(lastMove.from);
    }
    
    // Switch player back
    currentPlayer = (currentPlayer == Color::WHITE) ? Color::BLACK : Color::WHITE;
    
    return true;
}

// Get all legal moves for a player
std::vector<Move> ChessBoard::getAllLegalMoves(Color color) const {
    std::vector<Move> legalMoves;
    
    // Check every square for pieces of the given color
    for (int fromRow = 0; fromRow < 8; fromRow++) {
        for (int fromCol = 0; fromCol < 8; fromCol++) {
            Piece* piece = board[fromRow][fromCol];
            if (piece != nullptr && piece->getColor() == color) {
                Position from(fromRow, fromCol);
                
                // Check every possible destination
                for (int toRow = 0; toRow < 8; toRow++) {
                    for (int toCol = 0; toCol < 8; toCol++) {
                        Position to(toRow, toCol);
                        
                        // Check if this move is legal
                        if (isMoveLegal(from, to)) {
                            legalMoves.push_back(Move(from, to, getPieceAt(to)));
                        }
                    }
                }
            }
        }
    }
    
    return legalMoves;
}

// Get a random legal move for a player (for AI)
Move ChessBoard::getRandomMove(Color color) const {
    std::vector<Move> legalMoves = getAllLegalMoves(color);
    
    if (legalMoves.empty()) {
        return Move();  // No moves available
    }
    
    // Pick a random move
    int randomIndex = std::rand() % legalMoves.size();
    return legalMoves[randomIndex];
}

// Check if game is over (checkmate or stalemate)
bool ChessBoard::isGameOver() const {
    return isCheckmate(currentPlayer) || isStalemate(currentPlayer);
}

// Check for checkmate
bool ChessBoard::isCheckmate(Color color) const {
    // Must be in check AND have no legal moves
    return isInCheck(color) && getAllLegalMoves(color).empty();
}

// Check for stalemate
bool ChessBoard::isStalemate(Color color) const {
    // NOT in check BUT have no legal moves
    return !isInCheck(color) && getAllLegalMoves(color).empty();
}

// Print board to console (for debugging)
void ChessBoard::printBoard() const {
    std::cout << "\n  a b c d e f g h\n";
    std::cout << "  ----------------\n";
    
    for (int row = 0; row < 8; row++) {
        std::cout << 8 - row << "|";
        for (int col = 0; col < 8; col++) {
            Piece* piece = board[row][col];
            if (piece != nullptr) {
                std::cout << piece->getSymbol() << " ";
            } else {
                std::cout << ". ";
            }
        }
        std::cout << "|" << 8 - row << "\n";
    }
    
    std::cout << "  ----------------\n";
    std::cout << "  a b c d e f g h\n\n";
    
    // Show game status
    std::cout << "Current player: " 
              << (currentPlayer == Color::WHITE ? "WHITE" : "BLACK") << "\n";
    
    if (isInCheck(currentPlayer)) {
        std::cout << (currentPlayer == Color::WHITE ? "WHITE" : "BLACK") 
                  << " is in CHECK!\n";
    }
    
    if (isCheckmate(currentPlayer)) {
        std::cout << "CHECKMATE! " 
                  << (currentPlayer == Color::WHITE ? "BLACK" : "WHITE") 
                  << " wins!\n";
    } else if (isStalemate(currentPlayer)) {
        std::cout << "STALEMATE! Game is a draw.\n";
    }
}
void ChessBoard::resetBoard()
{
    // Clear existing pieces
    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            if (board[row][col]) {
                delete board[row][col];
                board[row][col] = nullptr;
            }
        }
    }

    // Reset game state
    currentPlayer = Color::WHITE;
    moveHistory.clear();

    // Initialize fresh board
    initializeBoard();
}
