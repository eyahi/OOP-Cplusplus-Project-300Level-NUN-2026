#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <vector>
#include <memory>
#include "Piece.h"

// Forward declaration
struct Move {
    Position from;
    Position to;
    Piece* capturedPiece;  // Piece that was captured (if any)
    
    Move(Position f = Position(), Position t = Position(), Piece* captured = nullptr)
        : from(f), to(t), capturedPiece(captured) {}
};

/**
 * @class ChessBoard
 * @brief Represents the chess board and game state
 * 
 * This class manages the 8x8 chess board, pieces, game state,
 * and validates moves according to chess rules.
 */
class ChessBoard {
private:
    Piece* board[8][8];           // 8x8 array of piece pointers
    Color currentPlayer;          // Whose turn it is (WHITE starts)
    std::vector<Move> moveHistory; // History of all moves made
    
    // Private helper functions
    void initializeBoard();
    void clearBoard();
    bool isSquareUnderAttack(Position square, Color attackerColor) const;
    
public:
    /**
     * @brief Constructor - creates a new chess board with initial setup
     */
    ChessBoard();
    
    /**
     * @brief Destructor - cleans up all pieces
     */
    ~ChessBoard();

    /**
     * @brief Reset the board to initial state
     */
    void resetBoard();
    
    void clearSelection();

    /**
     * @brief Copy constructor (for board copying)
     */
    ChessBoard(const ChessBoard& other);
    
    /**
     * @brief Assignment operator
     */
    ChessBoard& operator=(const ChessBoard& other);
    
    // Game state queries
    Color getCurrentPlayer() const { return currentPlayer; }
    bool isGameOver() const;
    bool isInCheck(Color color) const;
    bool isCheckmate(Color color) const;
    bool isStalemate(Color color) const;
    
    // Piece access
    Piece* getPieceAt(int row, int col) const;
    Piece* getPieceAt(Position pos) const;
    
    // Move management
    bool makeMove(Position from, Position to);
    bool undoLastMove();
    std::vector<Move> getAllLegalMoves(Color color) const;
    
    // AI helper
    Move getRandomMove(Color color) const;
    
    // Board display (for debugging)
    void printBoard() const;
    
private:
    /**
     * @brief Check if a move is legal (considering check)
     */
    bool isMoveLegal(Position from, Position to) const;
    
    /**
     * @brief Actually execute a move (internal use)
     */
    void executeMove(Position from, Position to);
};

#endif // CHESSBOARD_H
