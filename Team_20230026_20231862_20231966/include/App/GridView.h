#pragma once

#include "App/Controller.h"

#include <array>
#include <cstddef>
#include <cstdint>

#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>

namespace App {

class GridView {
public:
    GridView() = default;
    ~GridView();

    GridView(const GridView&) = delete;
    GridView& operator=(const GridView&) = delete;

    bool create(HWND parent, HINSTANCE hInst, int x, int y, int w, int h, Controller* controller);
    void destroy();

    [[nodiscard]] HWND hwnd() const { return m_hwnd; }

    void refreshFromModel(); // update texts, readonly, and coloring

private:
    static constexpr int IdBase = 1000;

    static LRESULT CALLBACK WndProcThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    static LRESULT CALLBACK EditProcThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT editProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void createEdits();
    void layoutEdits(int w, int h);
    void rebuildConflictMap();
    void setEditText(std::size_t r, std::size_t c, std::uint8_t v);

    [[nodiscard]] std::size_t indexFromHwnd(HWND edit) const;
    [[nodiscard]] bool cellFromId(int id, std::size_t& outR, std::size_t& outC) const;

    HWND m_hwnd{nullptr};
    HWND m_parent{nullptr};
    HINSTANCE m_hInst{nullptr};
    Controller* m_controller{nullptr};

    std::array<HWND, 81> m_edits{};
    std::array<WNDPROC, 81> m_editOldProc{};

    HFONT m_font{nullptr};
    HBRUSH m_brushGiven{nullptr};
    HBRUSH m_brushNormal{nullptr};
    HBRUSH m_brushConflict{nullptr};
    HBRUSH m_brushGrid{nullptr};

    bool m_syncing{false};
    bool m_conflict[9][9]{};
};

} // namespace App

