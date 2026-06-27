#include "App/Controller.h"

namespace App {

Controller::Controller() { newSamplePuzzle(); }

void Controller::newSamplePuzzle() {
    // '.' = empty. This puzzle has a unique solution.
    const std::string puzzle =
        "53..7...."
        "6..195..."
        ".98....6."
        "8...6...3"
        "4..8.3..1"
        "7...2...6"
        ".6....28."
        "...419..5"
        "....8..79";

    if (m_board.loadFromString(puzzle, /*markAsGiven=*/true)) {
        setStatus(L"Sample puzzle loaded.");
    } else {
        setStatus(L"Failed to load puzzle.");
    }
}

void Controller::clearAll() {
    m_board.clear();
    setStatus(L"Cleared board.");
}

void Controller::clearUserEntries() {
    m_board.clearNonGiven();
    setStatus(L"Cleared user entries.");
}

bool Controller::checkConflicts() {
    if (m_board.hasAnyConflicts()) {
        setStatus(L"Conflicts found.");
        return false;
    }
    setStatus(L"No conflicts.");
    return true;
}

bool Controller::solvePuzzle() {
    if (m_board.hasAnyConflicts()) {
        setStatus(L"Fix conflicts first.");
        return false;
    }

    const bool ok = m_solver.solve(m_board);
    if (ok) {
        setStatus(L"Solved.");
    } else {
        setStatus(L"No solution found.");
    }
    return ok;
}

void Controller::setCell(std::size_t r, std::size_t c, std::uint8_t v) {
    if (!m_board.setUserValue(r, c, v)) {
        // usually means "given" or invalid; UI should prevent most cases.
        setStatus(L"Can't edit that cell.");
        return;
    }

    if (m_board.hasAnyConflicts()) {
        setStatus(L"Conflicts detected.");
    } else if (m_board.isSolved()) {
        setStatus(L"Nice! You solved it.");
    } else {
        setStatus(L"");
    }
}

} // namespace App

