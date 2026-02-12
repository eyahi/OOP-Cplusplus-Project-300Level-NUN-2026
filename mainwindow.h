#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QLabel>
#include <QStyledItemDelegate>
#include "sudoku.h"

class DigitDelegate : public QStyledItemDelegate
{
public:
    explicit DigitDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const override;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    QTableWidget *grid = nullptr;
    QPushButton *btnNew = nullptr;
    QPushButton *btnCheck = nullptr;
    QPushButton *btnSolve = nullptr;
    QPushButton *btnClear = nullptr;
    QLabel *status = nullptr;

    Sudoku sudoku;
    Sudoku::Board puzzleBoard{};
    bool fixed[9][9] = {{false}};

    void setupUI();
    void initGrid();
    void loadPuzzle(const Sudoku::Board &b);

    Sudoku::Board readBoardFromUI() const;
    void writeBoardToUI(const Sudoku::Board &b);

    void markFixedCells(const Sudoku::Board &b);
    void refreshHighlights();

private slots:
    void newPuzzle();
    void checkPuzzle();
    void solvePuzzle();
    void clearNonFixed();
};
