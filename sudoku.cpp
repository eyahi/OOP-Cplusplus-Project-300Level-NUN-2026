#include "sudoku.h"
#include <algorithm>

Sudoku::Sudoku()
{
    std::random_device rd;
    rng = std::mt19937(rd());
}

void Sudoku::shuffleArray(std::array<int, 9> &arr)
{
    std::shuffle(arr.begin(), arr.end(), rng);
}

bool Sudoku::findEmpty(const Board &b, int &r, int &c) const
{
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            if (b[i][j] == 0) { r = i; c = j; return true; }
    return false;
}

bool Sudoku::isValidMove(const Board &b, int r, int c, int val) const
{
    if (val == 0) return true;
    if (val < 1 || val > 9) return false;

    for (int j = 0; j < 9; ++j)
        if (j != c && b[r][j] == val) return false;

    for (int i = 0; i < 9; ++i)
        if (i != r && b[i][c] == val) return false;

    int br = (r / 3) * 3;
    int bc = (c / 3) * 3;
    for (int i = br; i < br + 3; ++i)
        for (int j = bc; j < bc + 3; ++j)
            if (!(i == r && j == c) && b[i][j] == val) return false;

    return true;
}

bool Sudoku::rowOK(const Board &b, int r) const
{
    bool seen[10] = {false};
    for (int c = 0; c < 9; ++c) {
        int v = b[r][c];
        if (v == 0) continue;
        if (seen[v]) return false;
        seen[v] = true;
    }
    return true;
}

bool Sudoku::colOK(const Board &b, int c) const
{
    bool seen[10] = {false};
    for (int r = 0; r < 9; ++r) {
        int v = b[r][c];
        if (v == 0) continue;
        if (seen[v]) return false;
        seen[v] = true;
    }
    return true;
}

bool Sudoku::boxOK(const Board &b, int br, int bc) const
{
    bool seen[10] = {false};
    for (int r = br; r < br + 3; ++r) {
        for (int c = bc; c < bc + 3; ++c) {
            int v = b[r][c];
            if (v == 0) continue;
            if (seen[v]) return false;
            seen[v] = true;
        }
    }
    return true;
}

bool Sudoku::isBoardConsistent(const Board &b) const
{
    for (int r = 0; r < 9; ++r) if (!rowOK(b, r)) return false;
    for (int c = 0; c < 9; ++c) if (!colOK(b, c)) return false;
    for (int br = 0; br < 9; br += 3)
        for (int bc = 0; bc < 9; bc += 3)
            if (!boxOK(b, br, bc)) return false;
    return true;
}

bool Sudoku::solve(Board &b)
{
    return solveBacktrack(b);
}

bool Sudoku::solveBacktrack(Board &b)
{
    int r, c;
    if (!findEmpty(b, r, c)) return true;

    std::array<int, 9> nums = {1,2,3,4,5,6,7,8,9};
    shuffleArray(nums);

    for (int v : nums) {
        b[r][c] = v;
        if (isValidMove(b, r, c, v) && solveBacktrack(b))
            return true;
        b[r][c] = 0;
    }
    return false;
}

Sudoku::Board Sudoku::generateFullSolution()
{
    Board b{};
    for (auto &row : b) row.fill(0);
    solveBacktrack(b);
    return b;
}

Sudoku::Board Sudoku::generatePuzzle(int emptiesCount)
{
    if (emptiesCount < 0) emptiesCount = 0;
    if (emptiesCount > 81) emptiesCount = 81;

    Board full = generateFullSolution();
    Board puzzle = full;

    std::uniform_int_distribution<int> dist(0, 80);

    int removed = 0;
    int attempts = 0;

    while (removed < emptiesCount && attempts < 2500) {
        int idx = dist(rng);
        int r = idx / 9;
        int c = idx % 9;

        if (puzzle[r][c] == 0) { attempts++; continue; }

        int backup = puzzle[r][c];
        puzzle[r][c] = 0;

        Board temp = puzzle;
        if (!solve(temp)) {
            puzzle[r][c] = backup;
        } else {
            removed++;
        }
        attempts++;
    }

    return puzzle;
}
