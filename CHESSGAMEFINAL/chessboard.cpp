#include "chessboard.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QInputDialog>

ChessBoard::ChessBoard(QWidget *parent)
    : QWidget(parent), m_selectedPos(-1, -1), m_currentPlayer(WHITE), m_enPassantTarget(-1, -1)
{
    setFixedSize(SQUARE_SIZE * 8, SQUARE_SIZE * 8);
    initializeBoard();
}

void ChessBoard::initializeBoard()
{
    // Initialize empty board
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            m_board[i][j] = ChessPiece();
        }
    }

    // Set up white pieces
    m_board[7][0] = ChessPiece(ROOK, WHITE);
    m_board[7][1] = ChessPiece(KNIGHT, WHITE);
    m_board[7][2] = ChessPiece(BISHOP, WHITE);
    m_board[7][3] = ChessPiece(QUEEN, WHITE);
    m_board[7][4] = ChessPiece(KING, WHITE);
    m_board[7][5] = ChessPiece(BISHOP, WHITE);
    m_board[7][6] = ChessPiece(KNIGHT, WHITE);
    m_board[7][7] = ChessPiece(ROOK, WHITE);
    for (int i = 0; i < 8; ++i) {
        m_board[6][i] = ChessPiece(PAWN, WHITE);
    }

    // Set up black pieces
    m_board[0][0] = ChessPiece(ROOK, BLACK);
    m_board[0][1] = ChessPiece(KNIGHT, BLACK);
    m_board[0][2] = ChessPiece(BISHOP, BLACK);
    m_board[0][3] = ChessPiece(QUEEN, BLACK);
    m_board[0][4] = ChessPiece(KING, BLACK);
    m_board[0][5] = ChessPiece(BISHOP, BLACK);
    m_board[0][6] = ChessPiece(KNIGHT, BLACK);
    m_board[0][7] = ChessPiece(ROOK, BLACK);
    for (int i = 0; i < 8; ++i) {
        m_board[1][i] = ChessPiece(PAWN, BLACK);
    }
}

void ChessBoard::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    drawBoard(painter);
    drawPieces(painter);
}

void ChessBoard::drawBoard(QPainter &painter)
{
    QColor lightSquare(240, 217, 181);
    QColor darkSquare(181, 136, 99);
    QColor highlightColor(255, 255, 100, 180);
    QColor validMoveColor(100, 255, 100, 120);

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            QRect rect(col * SQUARE_SIZE, row * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);

            // Draw square
            if ((row + col) % 2 == 0) {
                painter.fillRect(rect, lightSquare);
            } else {
                painter.fillRect(rect, darkSquare);
            }

            // Highlight selected square
            if (m_selectedPos.isValid() && m_selectedPos.row == row && m_selectedPos.col == col) {
                painter.fillRect(rect, highlightColor);
            }

            // Highlight valid moves
            Position currentPos(row, col);
            for (const Position &validPos : m_validMoves) {
                if (validPos == currentPos) {
                    painter.setBrush(validMoveColor);
                    painter.setPen(Qt::NoPen);
                    int centerX = col * SQUARE_SIZE + SQUARE_SIZE / 2;
                    int centerY = row * SQUARE_SIZE + SQUARE_SIZE / 2;
                    painter.drawEllipse(QPoint(centerX, centerY), 12, 12);
                    break;
                }
            }
        }
    }
}

void ChessBoard::drawPieces(QPainter &painter)
{
    QFont font;
    font.setPointSize(48);
    painter.setFont(font);

    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (!m_board[row][col].isEmpty()) {
                QRect rect(col * SQUARE_SIZE, row * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE);
                painter.setPen(Qt::black);
                painter.drawText(rect, Qt::AlignCenter, m_board[row][col].getSymbol());
            }
        }
    }
}

void ChessBoard::mousePressEvent(QMouseEvent *event)
{
    Position clickedPos = getBoardPosition(event->x(), event->y());

    if (!clickedPos.isValid()) return;

    // If a piece is already selected
    if (m_selectedPos.isValid()) {
        // Check if clicked position is a valid move
        bool isValid = false;
        for (const Position &validPos : m_validMoves) {
            if (validPos == clickedPos) {
                isValid = true;
                break;
            }
        }

        if (isValid) {
            makeMove(m_selectedPos, clickedPos);
            m_selectedPos = Position(-1, -1);
            m_validMoves.clear();

            // Check game state
            if (isCheckmate(m_currentPlayer)) {
                QString winner = (m_currentPlayer == WHITE) ? "Black" : "White";
                QMessageBox::information(this, "Checkmate!", winner + " wins!");
            } else if (isStalemate(m_currentPlayer)) {
                QMessageBox::information(this, "Stalemate!", "The game is a draw!");
            } else if (isInCheck(m_currentPlayer)) {
                QMessageBox::information(this, "Check!",
                                         QString(m_currentPlayer == WHITE ? "White" : "Black") + " is in check!");
            }
        } else {
            // Select new piece if it belongs to current player
            if (!m_board[clickedPos.row][clickedPos.col].isEmpty() &&
                m_board[clickedPos.row][clickedPos.col].getColor() == m_currentPlayer) {
                m_selectedPos = clickedPos;
                m_validMoves = getValidMoves(clickedPos);
            } else {
                m_selectedPos = Position(-1, -1);
                m_validMoves.clear();
            }
        }
    } else {
        // Select piece if it belongs to current player
        if (!m_board[clickedPos.row][clickedPos.col].isEmpty() &&
            m_board[clickedPos.row][clickedPos.col].getColor() == m_currentPlayer) {
            m_selectedPos = clickedPos;
            m_validMoves = getValidMoves(clickedPos);
        }
    }

    update();
}

Position ChessBoard::getBoardPosition(int x, int y)
{
    int col = x / SQUARE_SIZE;
    int row = y / SQUARE_SIZE;
    return Position(row, col);
}

QVector<Position> ChessBoard::getValidMoves(const Position &pos)
{
    QVector<Position> moves;
    if (!pos.isValid()) return moves;

    ChessPiece piece = m_board[pos.row][pos.col];
    if (piece.isEmpty()) return moves;

    PieceType type = piece.getType();
    PieceColor color = piece.getColor();

    // Lambda to add move if valid
    auto addIfValid = [&](int newRow, int newCol) {
        Position newPos(newRow, newCol);
        if (newPos.isValid() && isValidMove(pos, newPos)) {
            moves.append(newPos);
        }
    };

    switch (type) {
    case PAWN: {
        int direction = (color == WHITE) ? -1 : 1;
        int startRow = (color == WHITE) ? 6 : 1;

        // Move forward
        Position forward(pos.row + direction, pos.col);
        if (forward.isValid() && m_board[forward.row][forward.col].isEmpty()) {
            addIfValid(forward.row, forward.col);

            // Double move from start
            if (pos.row == startRow) {
                Position doubleForward(pos.row + 2 * direction, pos.col);
                if (m_board[doubleForward.row][doubleForward.col].isEmpty()) {
                    addIfValid(doubleForward.row, doubleForward.col);
                }
            }
        }

        // Capture diagonally
        for (int dc : {-1, 1}) {
            Position capture(pos.row + direction, pos.col + dc);
            if (capture.isValid() &&
                (!m_board[capture.row][capture.col].isEmpty() || capture == m_enPassantTarget)) {
                addIfValid(capture.row, capture.col);
            }
        }
        break;
    }

    case KNIGHT: {
        int knightMoves[8][2] = {{-2,-1},{-2,1},{-1,-2},{-1,2},{1,-2},{1,2},{2,-1},{2,1}};
        for (auto &move : knightMoves) {
            addIfValid(pos.row + move[0], pos.col + move[1]);
        }
        break;
    }

    case BISHOP: {
        int directions[4][2] = {{-1,-1},{-1,1},{1,-1},{1,1}};
        for (auto &dir : directions) {
            for (int i = 1; i < 8; ++i) {
                int newRow = pos.row + i * dir[0];
                int newCol = pos.col + i * dir[1];
                Position newPos(newRow, newCol);
                if (!newPos.isValid()) break;
                if (!m_board[newRow][newCol].isEmpty()) {
                    addIfValid(newRow, newCol);
                    break;
                }
                addIfValid(newRow, newCol);
            }
        }
        break;
    }

    case ROOK: {
        int directions[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
        for (auto &dir : directions) {
            for (int i = 1; i < 8; ++i) {
                int newRow = pos.row + i * dir[0];
                int newCol = pos.col + i * dir[1];
                Position newPos(newRow, newCol);
                if (!newPos.isValid()) break;
                if (!m_board[newRow][newCol].isEmpty()) {
                    addIfValid(newRow, newCol);
                    break;
                }
                addIfValid(newRow, newCol);
            }
        }
        break;
    }

    case QUEEN: {
        int directions[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
        for (auto &dir : directions) {
            for (int i = 1; i < 8; ++i) {
                int newRow = pos.row + i * dir[0];
                int newCol = pos.col + i * dir[1];
                Position newPos(newRow, newCol);
                if (!newPos.isValid()) break;
                if (!m_board[newRow][newCol].isEmpty()) {
                    addIfValid(newRow, newCol);
                    break;
                }
                addIfValid(newRow, newCol);
            }
        }
        break;
    }

    case KING: {
        int directions[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
        for (auto &dir : directions) {
            addIfValid(pos.row + dir[0], pos.col + dir[1]);
        }

        // Castling
        if (!piece.hasMoved() && !isInCheck(color)) {
            // Kingside
            if (!m_board[pos.row][7].isEmpty() && !m_board[pos.row][7].hasMoved() &&
                m_board[pos.row][5].isEmpty() && m_board[pos.row][6].isEmpty()) {
                if (!wouldBeInCheck(pos, Position(pos.row, 5), color) &&
                    !wouldBeInCheck(pos, Position(pos.row, 6), color)) {
                    addIfValid(pos.row, 6);
                }
            }
            // Queenside
            if (!m_board[pos.row][0].isEmpty() && !m_board[pos.row][0].hasMoved() &&
                m_board[pos.row][1].isEmpty() && m_board[pos.row][2].isEmpty() &&
                m_board[pos.row][3].isEmpty()) {
                if (!wouldBeInCheck(pos, Position(pos.row, 3), color) &&
                    !wouldBeInCheck(pos, Position(pos.row, 2), color)) {
                    addIfValid(pos.row, 2);
                }
            }
        }
        break;
    }

    default:
        break;
    }

    return moves;
}

bool ChessBoard::isValidMove(const Position &from, const Position &to)
{
    if (!from.isValid() || !to.isValid()) return false;
    if (from == to) return false;

    ChessPiece piece = m_board[from.row][from.col];
    ChessPiece target = m_board[to.row][to.col];

    if (piece.isEmpty()) return false;
    if (!target.isEmpty() && target.getColor() == piece.getColor()) return false;

    // En passant special case
    if (piece.getType() == PAWN && to == m_enPassantTarget) {
        return !wouldBeInCheck(from, to, piece.getColor());
    }

    if (!target.isEmpty() || piece.getType() == PAWN) {
        // For captures and pawn moves, just check if it would result in check
        return !wouldBeInCheck(from, to, piece.getColor());
    }

    return !wouldBeInCheck(from, to, piece.getColor());
}

bool ChessBoard::wouldBeInCheck(const Position &from, const Position &to, PieceColor color)
{
    // Make temporary move
    ChessPiece tempFrom = m_board[from.row][from.col];
    ChessPiece tempTo = m_board[to.row][to.col];
    ChessPiece tempEnPassant;

    m_board[to.row][to.col] = tempFrom;
    m_board[from.row][from.col] = ChessPiece();

    // Handle en passant capture
    if (tempFrom.getType() == PAWN && to == m_enPassantTarget) {
        int captureRow = (color == WHITE) ? to.row + 1 : to.row - 1;
        tempEnPassant = m_board[captureRow][to.col];
        m_board[captureRow][to.col] = ChessPiece();
    }

    bool inCheck = isInCheck(color);

    // Restore board
    m_board[from.row][from.col] = tempFrom;
    m_board[to.row][to.col] = tempTo;

    if (tempFrom.getType() == PAWN && to == m_enPassantTarget) {
        int captureRow = (color == WHITE) ? to.row + 1 : to.row - 1;
        m_board[captureRow][to.col] = tempEnPassant;
    }

    return inCheck;
}

bool ChessBoard::isInCheck(PieceColor color)
{
    // Find king position
    Position kingPos(-1, -1);
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (m_board[row][col].getType() == KING &&
                m_board[row][col].getColor() == color) {
                kingPos = Position(row, col);
                break;
            }
        }
        if (kingPos.isValid()) break;
    }

    if (!kingPos.isValid()) return false;

    // Check if any opponent piece can attack the king
    PieceColor opponent = (color == WHITE) ? BLACK : WHITE;
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (m_board[row][col].getColor() == opponent) {
                Position pos(row, col);
                PieceType type = m_board[row][col].getType();

                // Check based on piece type
                if (type == PAWN) {
                    int direction = (opponent == WHITE) ? -1 : 1;
                    if ((kingPos.row == row + direction) &&
                        (kingPos.col == col - 1 || kingPos.col == col + 1)) {
                        return true;
                    }
                } else if (type == KNIGHT) {
                    int dr = abs(kingPos.row - row);
                    int dc = abs(kingPos.col - col);
                    if ((dr == 2 && dc == 1) || (dr == 1 && dc == 2)) {
                        return true;
                    }
                } else if (type == KING) {
                    if (abs(kingPos.row - row) <= 1 && abs(kingPos.col - col) <= 1) {
                        return true;
                    }
                } else {
                    // For sliding pieces (bishop, rook, queen)
                    int dr = kingPos.row - row;
                    int dc = kingPos.col - col;

                    bool checkThis = false;
                    if (type == BISHOP && abs(dr) == abs(dc)) checkThis = true;
                    else if (type == ROOK && (dr == 0 || dc == 0)) checkThis = true;
                    else if (type == QUEEN && (abs(dr) == abs(dc) || dr == 0 || dc == 0)) checkThis = true;

                    if (checkThis) {
                        int stepR = (dr == 0) ? 0 : (dr > 0 ? 1 : -1);
                        int stepC = (dc == 0) ? 0 : (dc > 0 ? 1 : -1);
                        int r = row + stepR;
                        int c = col + stepC;
                        bool blocked = false;
                        while (r != kingPos.row || c != kingPos.col) {
                            if (!m_board[r][c].isEmpty()) {
                                blocked = true;
                                break;
                            }
                            r += stepR;
                            c += stepC;
                        }
                        if (!blocked) return true;
                    }
                }
            }
        }
    }

    return false;
}

bool ChessBoard::isCheckmate(PieceColor color)
{
    if (!isInCheck(color)) return false;

    // Check if any move can get out of check
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (m_board[row][col].getColor() == color) {
                Position from(row, col);
                QVector<Position> moves = getValidMoves(from);
                if (!moves.isEmpty()) return false;
            }
        }
    }

    return true;
}

bool ChessBoard::isStalemate(PieceColor color)
{
    if (isInCheck(color)) return false;

    // Check if any legal move exists
    for (int row = 0; row < 8; ++row) {
        for (int col = 0; col < 8; ++col) {
            if (m_board[row][col].getColor() == color) {
                Position from(row, col);
                QVector<Position> moves = getValidMoves(from);
                if (!moves.isEmpty()) return false;
            }
        }
    }

    return true;
}

void ChessBoard::makeMove(const Position &from, const Position &to)
{
    ChessPiece piece = m_board[from.row][from.col];

    // Handle en passant capture
    if (piece.getType() == PAWN && to == m_enPassantTarget) {
        int captureRow = (piece.getColor() == WHITE) ? to.row + 1 : to.row - 1;
        m_board[captureRow][to.col] = ChessPiece();
    }

    // Handle castling
    if (piece.getType() == KING && abs(to.col - from.col) == 2) {
        if (to.col > from.col) { // Kingside
            m_board[to.row][5] = m_board[to.row][7];
            m_board[to.row][7] = ChessPiece();
            m_board[to.row][5].setMoved(true);
        } else { // Queenside
            m_board[to.row][3] = m_board[to.row][0];
            m_board[to.row][0] = ChessPiece();
            m_board[to.row][3].setMoved(true);
        }
    }

    // Update en passant target
    if (piece.getType() == PAWN && abs(to.row - from.row) == 2) {
        m_enPassantTarget = Position((from.row + to.row) / 2, from.col);
    } else {
        m_enPassantTarget = Position(-1, -1);
    }

    // Make the move
    m_board[to.row][to.col] = piece;
    m_board[from.row][from.col] = ChessPiece();
    m_board[to.row][to.col].setMoved(true);

    // Handle pawn promotion
    if (piece.getType() == PAWN && (to.row == 0 || to.row == 7)) {
        handlePromotion(to);
    }

    // Switch player
    m_currentPlayer = (m_currentPlayer == WHITE) ? BLACK : WHITE;
}

void ChessBoard::handlePromotion(const Position &pos)
{
    QStringList items;
    items << "Queen" << "Rook" << "Bishop" << "Knight";

    bool ok;
    QString choice = QInputDialog::getItem(this, "Pawn Promotion",
                                           "Choose piece:", items, 0, false, &ok);

    if (ok) {
        PieceType newType = QUEEN;
        if (choice == "Rook") newType = ROOK;
        else if (choice == "Bishop") newType = BISHOP;
        else if (choice == "Knight") newType = KNIGHT;

        m_board[pos.row][pos.col] = ChessPiece(newType, m_board[pos.row][pos.col].getColor());
    }
}
