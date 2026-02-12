#pragma once
#include <array>
#include <random>

class Sudoku
{
public:
    using Board = std::array<std::array<int, 9>, 9>;

    Sudoku();

    Board generatePuzzle(int emptiesCount = 50); // 40 easy, 50 medium, 60 hard
    bool solve(Board &b);

    bool isValidMove(const Board &b, int r, int c, int val) const;
    bool isBoardConsistent(const Board &b) const;

private:
    std::mt19937 rng;

    bool solveBacktrack(Board &b);
    bool findEmpty(const Board &b, int &r, int &c) const;

    Board generateFullSolution();

    bool rowOK(const Board &b, int r) const;
    bool colOK(const Board &b, int c) const;
    bool boxOK(const Board &b, int br, int bc) const;

    void shuffleArray(std::array<int, 9> &arr);
};
