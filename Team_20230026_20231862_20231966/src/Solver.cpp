#include "Sudoku/Solver.h"

namespace Sudoku {

bool Solver::solve(Board& board) {
    if (board.hasAnyConflicts()) return false;
    return solveBacktrack(board);
}

bool Solver::findEmpty(const Board& board, std::size_t& outR, std::size_t& outC) const {
    for (std::size_t r = 0; r < Board::Size; ++r) {
        for (std::size_t c = 0; c < Board::Size; ++c) {
            if (board.get(r, c) == 0) {
                outR = r;
                outC = c;
                return true;
            }
        }
    }
    return false;
}

bool Solver::solveBacktrack(Board& board) {
    std::size_t r = 0, c = 0;
    if (!findEmpty(board, r, c)) return true; // no empty -> solved

    for (std::uint8_t v = 1; v <= 9; ++v) {
        if (!board.isValidMove(r, c, v)) continue;
        board.setValue(r, c, v);
        if (solveBacktrack(board)) return true;
        board.setValue(r, c, 0);
    }

    return false;
}

} // namespace Sudoku

