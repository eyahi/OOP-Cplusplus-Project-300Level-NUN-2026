#include "Sudoku/Board.h"

#include <array>
#include <vector>

namespace Sudoku {

Board::Board() { clear(); }

void Board::clear() {
    for (auto& row : m_cells) {
        for (auto& cell : row) {
            cell.setValue(0);
            cell.setGiven(false);
        }
    }
}

void Board::clearNonGiven() {
    for (auto& row : m_cells) {
        for (auto& cell : row) {
            if (!cell.given()) cell.setValue(0);
        }
    }
}

bool Board::loadFromString(const std::string& s, bool markAsGiven) {
    if (s.size() != 81) return false;

    clear();

    for (std::size_t i = 0; i < 81; ++i) {
        const char ch = s[i];
        std::uint8_t v = 0;
        if (ch == '.' || ch == '0') {
            v = 0;
        } else if (ch >= '1' && ch <= '9') {
            v = static_cast<std::uint8_t>(ch - '0');
        } else {
            return false;
        }

        const std::size_t r = i / 9;
        const std::size_t c = i % 9;
        m_cells[r][c].setValue(v);
        m_cells[r][c].setGiven(markAsGiven && (v != 0));
    }

    return true;
}

std::uint8_t Board::get(std::size_t r, std::size_t c) const {
    if (!isValidCoord(r, c)) return 0;
    return m_cells[r][c].value();
}

bool Board::isGiven(std::size_t r, std::size_t c) const {
    if (!isValidCoord(r, c)) return false;
    return m_cells[r][c].given();
}

bool Board::setUserValue(std::size_t r, std::size_t c, std::uint8_t v) {
    if (!isValidCoord(r, c)) return false;
    if (m_cells[r][c].given()) return false;
    if (v > 9) return false;
    m_cells[r][c].setValue(v);
    return true;
}

void Board::setValue(std::size_t r, std::size_t c, std::uint8_t v) {
    if (!isValidCoord(r, c)) return;
    if (v > 9) return;
    m_cells[r][c].setValue(v);
}

bool Board::isValidMove(std::size_t r, std::size_t c, std::uint8_t v) const {
    if (!isValidCoord(r, c)) return false;
    if (v == 0) return true;
    if (v > 9) return false;

    // Row
    for (std::size_t cc = 0; cc < Size; ++cc) {
        if (cc == c) continue;
        if (m_cells[r][cc].value() == v) return false;
    }

    // Col
    for (std::size_t rr = 0; rr < Size; ++rr) {
        if (rr == r) continue;
        if (m_cells[rr][c].value() == v) return false;
    }

    // Block
    const std::size_t br = (r / 3) * 3;
    const std::size_t bc = (c / 3) * 3;
    for (std::size_t rr = br; rr < br + 3; ++rr) {
        for (std::size_t cc = bc; cc < bc + 3; ++cc) {
            if (rr == r && cc == c) continue;
            if (m_cells[rr][cc].value() == v) return false;
        }
    }

    return true;
}

std::vector<Conflict> Board::conflicts() const {
    bool bad[Size][Size]{};

    // Rows
    for (std::size_t r = 0; r < Size; ++r) {
        for (std::size_t c1 = 0; c1 < Size; ++c1) {
            const auto v = m_cells[r][c1].value();
            if (v == 0) continue;
            for (std::size_t c2 = c1 + 1; c2 < Size; ++c2) {
                if (m_cells[r][c2].value() == v) {
                    bad[r][c1] = true;
                    bad[r][c2] = true;
                }
            }
        }
    }

    // Cols
    for (std::size_t c = 0; c < Size; ++c) {
        for (std::size_t r1 = 0; r1 < Size; ++r1) {
            const auto v = m_cells[r1][c].value();
            if (v == 0) continue;
            for (std::size_t r2 = r1 + 1; r2 < Size; ++r2) {
                if (m_cells[r2][c].value() == v) {
                    bad[r1][c] = true;
                    bad[r2][c] = true;
                }
            }
        }
    }

    // Blocks
    for (std::size_t br = 0; br < 3; ++br) {
        for (std::size_t bc = 0; bc < 3; ++bc) {
            std::array<std::pair<std::size_t, std::size_t>, 9> coords{};
            std::size_t k = 0;
            for (std::size_t r = br * 3; r < br * 3 + 3; ++r) {
                for (std::size_t c = bc * 3; c < bc * 3 + 3; ++c) {
                    coords[k++] = {r, c};
                }
            }
            for (std::size_t i = 0; i < coords.size(); ++i) {
                const std::size_t r1 = coords[i].first;
                const std::size_t c1 = coords[i].second;
                const auto v = m_cells[r1][c1].value();
                if (v == 0) continue;
                for (std::size_t j = i + 1; j < coords.size(); ++j) {
                    const std::size_t r2 = coords[j].first;
                    const std::size_t c2 = coords[j].second;
                    if (m_cells[r2][c2].value() == v) {
                        bad[r1][c1] = true;
                        bad[r2][c2] = true;
                    }
                }
            }
        }
    }

    std::vector<Conflict> out;
    out.reserve(16);
    for (std::size_t r = 0; r < Size; ++r) {
        for (std::size_t c = 0; c < Size; ++c) {
            if (bad[r][c]) out.push_back({r, c});
        }
    }
    return out;
}

bool Board::hasAnyConflicts() const { return !conflicts().empty(); }

bool Board::isSolved() const {
    for (std::size_t r = 0; r < Size; ++r) {
        for (std::size_t c = 0; c < Size; ++c) {
            if (m_cells[r][c].value() == 0) return false;
        }
    }
    return !hasAnyConflicts();
}

bool Board::isValidCoord(std::size_t r, std::size_t c) const { return r < Size && c < Size; }

} // namespace Sudoku

