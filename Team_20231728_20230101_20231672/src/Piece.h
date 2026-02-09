#ifndef PIECE_H
#define PIECE_H

#include <string>

/**
 * @file Piece.h
 * @author Chess Game Developer
 * @date 2026
 * @brief Base class for all chess pieces
 * @course Object Oriented Programming - Chess Game Project
 */

// Enum for piece colors
enum class Color { WHITE, BLACK };

// Simple struct for board position (row 0-7, col 0-7)
struct Position {
    int row;  // 0-7 (0 = top, 7 = bottom)
    int col;  // 0-7 (0 = left, 7 = right)
    
    Position(int r = -1, int c = -1) : row(r), col(c) {}
    
    bool operator==(const Position& other) const {
        return row == other.row && col == other.col;
    }
    
    bool isValid() const {
        return row >= 0 && row < 8 && col >= 0 && col < 8;
    }
};

/**
 * @class Piece
 * @brief Abstract base class for all chess pieces
 * 
 * This class defines the interface for chess pieces using inheritance.
 * Each specific piece (Pawn, Knight, etc.) will inherit from this class
 * and implement its own movement rules.
 */
class Piece {
protected:
    Color pieceColor;      // Color of the piece (WHITE or BLACK)
    Position position;     // Current position on the board
    char symbol;           // Character symbol for display (e.g., 'P' for pawn)
    bool hasMoved;         // Track if piece has moved (for castling/en passant)
    
public:
    /**
     * @brief Constructor for Piece
     * @param color Color of the piece (WHITE or BLACK)
     * @param pos Initial position on the board
     * @param sym Character symbol for display
     */
    Piece(Color color, Position pos, char sym) 
        : pieceColor(color), position(pos), symbol(sym), hasMoved(false) {}
    
    /**
     * @brief Virtual destructor for proper cleanup of derived classes
     */
    virtual ~Piece() {}
    
    /**
     * @brief Get the color of the piece
     * @return Color (WHITE or BLACK)
     */
    Color getColor() const { return pieceColor; }
    
    /**
     * @brief Get the current position of the piece
     * @return Position struct with row and col
     */
    Position getPosition() const { return position; }
    
    /**
     * @brief Set the position of the piece
     * @param pos New position on the board
     */
    void setPosition(Position pos) { 
        position = pos; 
        hasMoved = true;  // Mark as moved when position changes
    }
    
    /**
     * @brief Get the display symbol of the piece
     * @return Character symbol (e.g., 'P', 'N', 'K')
     */
    char getSymbol() const { return symbol; }
    
    /**
     * @brief Check if piece has moved (for special moves)
     * @return true if piece has moved at least once
     */
    bool getHasMoved() const { return hasMoved; }
    
    /**
     * @brief Pure virtual function to check if a move is valid
     * 
     * Each derived class must implement its own movement rules.
     * 
     * @param to Destination position
     * @param board Reference to the chess board for checking other pieces
     * @return true if the move is valid according to piece rules
     */
    virtual bool isValidMove(Position to, class ChessBoard& board) const = 0;
    
    /**
     * @brief Virtual function to get piece type as string
     * @return String representation of piece type (e.g., "Pawn", "Knight")
     */
    virtual std::string getType() const = 0;
    
    /**
     * @brief Create a deep copy of the piece (for board copying)
     * @return Pointer to new Piece object (caller must delete)
     */
    virtual Piece* clone() const = 0;
    
protected:
    /**
     * @brief Helper function to check if position is within board bounds
     * @param pos Position to check
     * @return true if position is valid (0-7 for row and col)
     */
    bool isPositionValid(Position pos) const {
        return pos.row >= 0 && pos.row < 8 && pos.col >= 0 && pos.col < 8;
    }
    
    /**
     * @brief Helper function to check if squares form a clear path (for sliding pieces)
     * @param from Starting position
     * @param to Ending position
     * @param board Reference to the chess board
     * @return true if path is clear of other pieces
     */
    bool isPathClear(Position from, Position to, class ChessBoard& board) const;
};

#endif // PIECE_H