#pragma once

#include "Sudoku/Cell.h"

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace Sudoku {

struct Conflict {
    std::size_t row{};
    std::size_t col{};
};

class Board {
public:
    static constexpr std::size_t Size = 9;

    Board();

    void clear();
    void clearNonGiven();

    // Accepts 81 chars: '0' or '.' = empty, '1'..'9' filled.
    // If markAsGiven=true, any non-empty is treated as fixed clue.
    bool loadFromString(const std::string& s, bool markAsGiven);

    [[nodiscard]] std::uint8_t get(std::size_t r, std::size_t c) const;
    [[nodiscard]] bool isGiven(std::size_t r, std::size_t c) const;

    // For UI edits. Returns false if cell is given or value invalid.
    bool setUserValue(std::size_t r, std::size_t c, std::uint8_t v);

    // For solver. Ignores "given" rule.
    void setValue(std::size_t r, std::size_t c, std::uint8_t v);

    [[nodiscard]] bool isValidMove(std::size_t r, std::size_t c, std::uint8_t v) const;
    [[nodiscard]] bool hasAnyConflicts() const;
    [[nodiscard]] std::vector<Conflict> conflicts() const;
    [[nodiscard]] bool isSolved() const;

private:
    [[nodiscard]] bool isValidCoord(std::size_t r, std::size_t c) const;

    std::array<std::array<Cell, Size>, Size> m_cells{};
};

} // namespace Sudoku

