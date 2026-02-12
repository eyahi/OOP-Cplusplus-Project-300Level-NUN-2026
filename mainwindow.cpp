#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QSignalBlocker>
#include <QFont>
#include <QLineEdit>
#include <QIntValidator>

// ---------------- DigitDelegate ----------------

QWidget *DigitDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem &,
                                     const QModelIndex &) const
{
    auto *edit = new QLineEdit(parent);
    edit->setAlignment(Qt::AlignCenter);

    // allow empty or 1..9; we validate in setModelData too
    auto *validator = new QIntValidator(1, 9, edit);
    edit->setValidator(validator);
    edit->setMaxLength(1);
    return edit;
}

void DigitDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    auto *edit = qobject_cast<QLineEdit*>(editor);
    if (!edit) return;

    QString text = index.data(Qt::EditRole).toString();
    edit->setText(text);
}

void DigitDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                 const QModelIndex &index) const
{
    auto *edit = qobject_cast<QLineEdit*>(editor);
    if (!edit) return;

    QString t = edit->text().trimmed();

    // allow empty (clears cell)
    if (t.isEmpty()) {
        model->setData(index, "", Qt::EditRole);
        return;
    }

    bool ok = false;
    int v = t.toInt(&ok);

    if (!ok || v < 1 || v > 9) {
        model->setData(index, "", Qt::EditRole);
        return;
    }

    model->setData(index, QString::number(v), Qt::EditRole);
}

// ---------------- MainWindow ----------------

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    newPuzzle();
}

void MainWindow::setupUI()
{
    auto *central = new QWidget(this);
    setCentralWidget(central);

    grid = new QTableWidget(9, 9, this);
    btnNew = new QPushButton("New Puzzle", this);
    btnCheck = new QPushButton("Check", this);
    btnSolve = new QPushButton("Solve", this);
    btnClear = new QPushButton("Clear", this);
    status = new QLabel("Ready", this);

    auto *buttons = new QHBoxLayout();
    buttons->addWidget(btnNew);
    buttons->addWidget(btnCheck);
    buttons->addWidget(btnSolve);
    buttons->addWidget(btnClear);

    auto *layout = new QVBoxLayout();
    layout->addWidget(grid);
    layout->addLayout(buttons);
    layout->addWidget(status);
    central->setLayout(layout);

    initGrid();

    connect(btnNew, &QPushButton::clicked, this, &MainWindow::newPuzzle);
    connect(btnCheck, &QPushButton::clicked, this, &MainWindow::checkPuzzle);
    connect(btnSolve, &QPushButton::clicked, this, &MainWindow::solvePuzzle);
    connect(btnClear, &QPushButton::clicked, this, &MainWindow::clearNonFixed);

    setWindowTitle("Sudoku (Qt Widgets)");
    resize(520, 620);
}

void MainWindow::initGrid()
{
    grid->horizontalHeader()->setVisible(false);
    grid->verticalHeader()->setVisible(false);

    grid->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    grid->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    grid->setSelectionMode(QAbstractItemView::SingleSelection);
    grid->setSelectionBehavior(QAbstractItemView::SelectItems);
    grid->setEditTriggers(QAbstractItemView::AllEditTriggers);

    // set a delegate that restricts editing to 1..9
    grid->setItemDelegate(new DigitDelegate(grid));

    QFont f = grid->font();
    f.setPointSize(18);
    f.setBold(true);
    grid->setFont(f);

    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            auto *item = new QTableWidgetItem("");
            item->setTextAlignment(Qt::AlignCenter);
            grid->setItem(r, c, item);
        }
    }
}

void MainWindow::markFixedCells(const Sudoku::Board &b)
{
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            fixed[r][c] = (b[r][c] != 0);
}

void MainWindow::loadPuzzle(const Sudoku::Board &b)
{
    puzzleBoard = b;
    markFixedCells(b);
    writeBoardToUI(b);
    refreshHighlights();
    status->setText("New puzzle loaded.");
}

Sudoku::Board MainWindow::readBoardFromUI() const
{
    Sudoku::Board b{};
    for (auto &row : b) row.fill(0);

    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            auto *it = grid->item(r, c);
            if (!it) continue;

            QString t = it->text().trimmed();
            if (t.isEmpty()) { b[r][c] = 0; continue; }

            bool ok = false;
            int v = t.toInt(&ok);
            b[r][c] = ok ? v : 0;
        }
    }
    return b;
}

void MainWindow::writeBoardToUI(const Sudoku::Board &b)
{
    QSignalBlocker blocker(grid);

    const QPalette pal = this->palette();
    const QColor baseBg = pal.color(QPalette::Base);
    const QColor fixedBg = pal.color(QPalette::AlternateBase);
    const QColor textFg = pal.color(QPalette::Text);

    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            auto *it = grid->item(r, c);
            if (!it) continue;

            it->setText(b[r][c] == 0 ? "" : QString::number(b[r][c]));
            it->setForeground(QBrush(textFg));

            if (fixed[r][c]) {
                it->setFlags(it->flags() & ~Qt::ItemIsEditable);
                it->setBackground(QBrush(fixedBg));
            } else {
                it->setFlags(it->flags() | Qt::ItemIsEditable);
                it->setBackground(QBrush(baseBg));
            }
        }
    }
}

void MainWindow::refreshHighlights()
{
    Sudoku::Board b = readBoardFromUI();

    const QPalette pal = this->palette();
    const QColor baseBg = pal.color(QPalette::Base);
    const QColor fixedBg = pal.color(QPalette::AlternateBase);
    const QColor textFg = pal.color(QPalette::Text);

    // reset colors
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            auto *it = grid->item(r, c);
            if (!it) continue;

            it->setForeground(QBrush(textFg));
            it->setBackground(fixed[r][c] ? QBrush(fixedBg) : QBrush(baseBg));
        }
    }

    // conflict highlight (force readable text)
    for (int r = 0; r < 9; ++r) {
        for (int c = 0; c < 9; ++c) {
            int v = b[r][c];
            if (v == 0) continue;

            if (!sudoku.isValidMove(b, r, c, v)) {
                auto *it = grid->item(r, c);
                if (!it) continue;
                it->setBackground(QBrush(QColor(255, 190, 190)));
                it->setForeground(QBrush(Qt::black));
            }
        }
    }
}

void MainWindow::newPuzzle()
{
    // difficulty: 40 easy, 50 medium, 60 hard
    Sudoku::Board b = sudoku.generatePuzzle(50);
    loadPuzzle(b);
}

void MainWindow::checkPuzzle()
{
    Sudoku::Board b = readBoardFromUI();

    if (!sudoku.isBoardConsistent(b)) {
        status->setText("Conflicts found.");
        QMessageBox::warning(this, "Check", "Conflicts exist. Fix the red cells.");
        refreshHighlights();
        return;
    }

    bool complete = true;
    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            if (b[r][c] == 0) complete = false;

    if (!complete) {
        status->setText("No conflicts (incomplete).");
        QMessageBox::information(this, "Check", "No conflicts so far — keep going!");
        return;
    }

    status->setText("Solved!");
    QMessageBox::information(this, "Check", "Solved ✅");
}

void MainWindow::solvePuzzle()
{
    Sudoku::Board b = readBoardFromUI();

    if (!sudoku.isBoardConsistent(b)) {
        QMessageBox::warning(this, "Solve", "Fix conflicts first (red cells).");
        return;
    }

    Sudoku::Board temp = b;
    if (!sudoku.solve(temp)) {
        QMessageBox::warning(this, "Solve", "This puzzle seems unsolvable.");
        return;
    }

    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            if (!fixed[r][c]) b[r][c] = temp[r][c];

    writeBoardToUI(b);
    refreshHighlights();
    status->setText("Solved.");
}

void MainWindow::clearNonFixed()
{
    Sudoku::Board b = readBoardFromUI();

    for (int r = 0; r < 9; ++r)
        for (int c = 0; c < 9; ++c)
            if (!fixed[r][c]) b[r][c] = 0;

    writeBoardToUI(b);
    refreshHighlights();
    status->setText("Cleared.");
}
