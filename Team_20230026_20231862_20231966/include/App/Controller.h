#pragma once

#include "Sudoku/Board.h"
#include "Sudoku/Solver.h"

#include <cstddef>
#include <cstdint>
#include <string>

namespace App {

class Controller {
public:
    Controller();

    [[nodiscard]] const Sudoku::Board& board() const { return m_board; }

    // UI actions
    void newSamplePuzzle();
    void clearAll();
    void clearUserEntries();
    [[nodiscard]] bool checkConflicts();
    [[nodiscard]] bool solvePuzzle();

    // UI cell edit
    void setCell(std::size_t r, std::size_t c, std::uint8_t v);

    [[nodiscard]] std::wstring statusText() const { return m_status; }

private:
    void setStatus(const std::wstring& s) { m_status = s; }

    Sudoku::Board m_board{};
    Sudoku::Solver m_solver{};
    std::wstring m_status{};
};

} // namespace App

