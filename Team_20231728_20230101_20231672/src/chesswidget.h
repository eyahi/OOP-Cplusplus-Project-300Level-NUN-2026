#ifndef CHESSWIDGET_H
#define CHESSWIDGET_H

#include <QWidget>
#include "ChessBoard.h"
#include <vector>

class ChessWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChessWidget(QWidget *parent = nullptr);
    void setChessBoard(ChessBoard* board);
    void clearSelection();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

signals:
    void squareClicked(int row, int col);
    void boardStateChanged();

private:
    ChessBoard* chessBoard;
    QMap<QString, QPixmap> pieceImages;

    Position selectedPiece;  // Currently selected piece position (-1, -1 if none)
    std::vector<Position> possibleMoves;  // Valid moves for selected piece

    void loadPieceImages();
    QString getImageName(Piece* piece) const;
};

#endif // CHESSWIDGET_H
