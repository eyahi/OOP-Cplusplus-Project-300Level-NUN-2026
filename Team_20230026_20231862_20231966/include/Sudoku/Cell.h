#pragma once

#include <cstdint>

namespace Sudoku {

class Cell {
public:
    Cell() = default;
    explicit Cell(std::uint8_t value, bool given)
        : m_value(value), m_given(given) {}

    [[nodiscard]] std::uint8_t value() const { return m_value; } // 0..9 (0 = empty)
    [[nodiscard]] bool given() const { return m_given; }

    void setValue(std::uint8_t v) { m_value = v; }
    void setGiven(bool g) { m_given = g; }

private:
    std::uint8_t m_value{0};
    bool m_given{false};
};

} // namespace Sudoku

