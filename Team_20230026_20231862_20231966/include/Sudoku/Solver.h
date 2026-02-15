#pragma once

#include "Sudoku/Board.h"

namespace Sudoku {

class Solver {
public:
    // Solves in-place. Returns true if solved.
    bool solve(Board& board);

private:
    bool solveBacktrack(Board& board);
    bool findEmpty(const Board& board, std::size_t& outR, std::size_t& outC) const;
};

} // namespace Sudoku

