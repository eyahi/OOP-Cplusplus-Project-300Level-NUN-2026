#include "App/GridView.h"

#include <algorithm>
#include <string>

namespace App {

namespace {
constexpr wchar_t kGridClassName[] = L"SudokuGridView";
}

GridView::~GridView() { destroy(); }

bool GridView::create(HWND parent, HINSTANCE hInst, int x, int y, int w, int h, Controller* controller) {
    m_parent = parent;
    m_hInst = hInst;
    m_controller = controller;

    static bool registered = false;
    if (!registered) {
        WNDCLASSEXW wc{};
        wc.cbSize = sizeof(wc);
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc = &GridView::WndProcThunk;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInst;
        wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        wc.hbrBackground = nullptr; // we paint/erase ourselves
        wc.lpszClassName = kGridClassName;
        if (!RegisterClassExW(&wc)) return false;
        registered = true;
    }

    m_hwnd = CreateWindowExW(
        0,
        kGridClassName,
        L"",
        WS_CHILD | WS_VISIBLE,
        x,
        y,
        w,
        h,
        parent,
        nullptr,
        hInst,
        this);

    return m_hwnd != nullptr;
}

void GridView::destroy() {
    if (m_font) {
        DeleteObject(m_font);
        m_font = nullptr;
    }
    if (m_brushGiven) {
        DeleteObject(m_brushGiven);
        m_brushGiven = nullptr;
    }
    if (m_brushNormal) {
        DeleteObject(m_brushNormal);
        m_brushNormal = nullptr;
    }
    if (m_brushConflict) {
        DeleteObject(m_brushConflict);
        m_brushConflict = nullptr;
    }
    if (m_brushGrid) {
        DeleteObject(m_brushGrid);
        m_brushGrid = nullptr;
    }

    if (m_hwnd) {
        DestroyWindow(m_hwnd);
        m_hwnd = nullptr;
    }
}

LRESULT CALLBACK GridView::WndProcThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    GridView* self = reinterpret_cast<GridView*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    if (msg == WM_NCCREATE) {
        auto* cs = reinterpret_cast<CREATESTRUCTW*>(lParam);
        self = reinterpret_cast<GridView*>(cs->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
        if (self) self->m_hwnd = hwnd;
    }
    if (!self) return DefWindowProcW(hwnd, msg, wParam, lParam);
    return self->wndProc(hwnd, msg, wParam, lParam);
}

LRESULT GridView::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            // Font
            LOGFONTW lf{};
            lf.lfHeight = -22;
            lf.lfWeight = FW_SEMIBOLD;
            lstrcpynW(lf.lfFaceName, L"Segoe UI", LF_FACESIZE);
            m_font = CreateFontIndirectW(&lf);

            // Brushes
            m_brushGiven = CreateSolidBrush(RGB(235, 235, 235));
            m_brushNormal = CreateSolidBrush(RGB(255, 255, 255));
            m_brushConflict = CreateSolidBrush(RGB(255, 225, 225));
            m_brushGrid = CreateSolidBrush(RGB(35, 35, 35));

            createEdits();
            refreshFromModel();
            return 0;
        }

        case WM_SIZE: {
            layoutEdits(LOWORD(lParam), HIWORD(lParam));
            return 0;
        }

        case WM_ERASEBKGND: {
            RECT rc{};
            GetClientRect(hwnd, &rc);
            FillRect(reinterpret_cast<HDC>(wParam), &rc, m_brushGrid ? m_brushGrid : reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
            return 1;
        }

        case WM_COMMAND: {
            if (HIWORD(wParam) == EN_CHANGE) {
                if (m_syncing || !m_controller) return 0;

                const int id = LOWORD(wParam);
                std::size_t r = 0, c = 0;
                if (!cellFromId(id, r, c)) return 0;

                HWND edit = reinterpret_cast<HWND>(lParam);

                wchar_t buf[8]{};
                GetWindowTextW(edit, buf, 8);
                std::uint8_t v = 0;
                if (buf[0] >= L'1' && buf[0] <= L'9' && buf[1] == 0) {
                    v = static_cast<std::uint8_t>(buf[0] - L'0');
                } else {
                    v = 0;
                }

                m_controller->setCell(r, c, v);
                rebuildConflictMap();
                InvalidateRect(hwnd, nullptr, TRUE);
                PostMessageW(m_parent, WM_APP + 1, 0, 0);
            }
            return 0;
        }

        case WM_CTLCOLOREDIT: {
            if (!m_controller) break;
            HDC hdc = reinterpret_cast<HDC>(wParam);
            HWND edit = reinterpret_cast<HWND>(lParam);

            const auto idx = indexFromHwnd(edit);
            const std::size_t r = idx / 9;
            const std::size_t c = idx % 9;

            SetBkMode(hdc, OPAQUE);

            if (m_conflict[r][c]) {
                SetTextColor(hdc, RGB(170, 0, 0));
                SetBkColor(hdc, RGB(255, 225, 225));
                return reinterpret_cast<INT_PTR>(m_brushConflict);
            }

            if (m_controller->board().isGiven(r, c)) {
                SetTextColor(hdc, RGB(0, 0, 0));
                SetBkColor(hdc, RGB(235, 235, 235));
                return reinterpret_cast<INT_PTR>(m_brushGiven);
            }

            SetTextColor(hdc, RGB(0, 70, 180));
            SetBkColor(hdc, RGB(255, 255, 255));
            return reinterpret_cast<INT_PTR>(m_brushNormal);
        }

        default:
            break;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

LRESULT CALLBACK GridView::EditProcThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    auto* self = reinterpret_cast<GridView*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    if (!self) return DefWindowProcW(hwnd, msg, wParam, lParam);
    return self->editProc(hwnd, msg, wParam, lParam);
}

LRESULT GridView::editProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    const auto idx = indexFromHwnd(hwnd);
    if (idx >= m_edits.size()) return DefWindowProcW(hwnd, msg, wParam, lParam);
    const WNDPROC oldProc = m_editOldProc[idx];

    switch (msg) {
        case WM_PASTE:
            return 0;

        case WM_KEYDOWN:
            if (wParam == VK_DELETE) {
                SetWindowTextW(hwnd, L"");
                return 0;
            }
            break;

        case WM_SETFOCUS: {
            const auto res = CallWindowProcW(oldProc, hwnd, msg, wParam, lParam);
            SendMessageW(hwnd, EM_SETSEL, 0, -1);
            return res;
        }

        case WM_CHAR: {
            const wchar_t ch = static_cast<wchar_t>(wParam);
            if (ch == VK_BACK) {
                return CallWindowProcW(oldProc, hwnd, msg, wParam, lParam);
            }

            if (ch >= L'1' && ch <= L'9') {
                wchar_t s[2]{ch, 0};
                SetWindowTextW(hwnd, s);
                SendMessageW(hwnd, EM_SETSEL, 1, 1);
                return 0;
            }

            if (ch == L'0' || ch == VK_DELETE) {
                SetWindowTextW(hwnd, L"");
                return 0;
            }

            return 0;
        }

        default:
            break;
    }

    return CallWindowProcW(oldProc, hwnd, msg, wParam, lParam);
}

void GridView::createEdits() {
    for (std::size_t i = 0; i < m_edits.size(); ++i) {
        const int id = IdBase + static_cast<int>(i);
        HWND edit = CreateWindowExW(
            0,
            L"EDIT",
            L"",
            WS_CHILD | WS_VISIBLE | ES_CENTER | ES_NUMBER,
            0,
            0,
            10,
            10,
            m_hwnd,
            reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
            m_hInst,
            nullptr);

        m_edits[i] = edit;

        SendMessageW(edit, WM_SETFONT, reinterpret_cast<WPARAM>(m_font), TRUE);
        SendMessageW(edit, EM_SETLIMITTEXT, 1, 0);

        SetWindowLongPtrW(edit, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
        m_editOldProc[i] = reinterpret_cast<WNDPROC>(
            SetWindowLongPtrW(edit, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&GridView::EditProcThunk)));
    }
}

void GridView::layoutEdits(int w, int h) {
    constexpr int pad = 10;
    constexpr int gap = 2;
    constexpr int blockGap = 7;

    const int totalGaps = 6 * gap + 2 * blockGap;
    const int usableW = std::max(0, w - 2 * pad - totalGaps);
    const int usableH = std::max(0, h - 2 * pad - totalGaps);
    int cell = std::min(usableW / 9, usableH / 9);
    if (cell < 26) cell = 26;
    if (cell > 64) cell = 64;

    const int boardW = 9 * cell + totalGaps;
    const int boardH = 9 * cell + totalGaps;
    const int startX = std::max(pad, (w - boardW) / 2);
    const int startY = std::max(pad, (h - boardH) / 2);

    int y = startY;
    for (int r = 0; r < 9; ++r) {
        int x = startX;
        for (int c = 0; c < 9; ++c) {
            const std::size_t idx = static_cast<std::size_t>(r * 9 + c);
            SetWindowPos(m_edits[idx], nullptr, x, y, cell, cell, SWP_NOZORDER | SWP_NOACTIVATE);
            x += cell + ((c == 2 || c == 5) ? blockGap : gap);
        }
        y += cell + ((r == 2 || r == 5) ? blockGap : gap);
    }
}

void GridView::rebuildConflictMap() {
    for (auto& row : m_conflict) {
        for (bool& v : row) v = false;
    }
    if (!m_controller) return;
    const auto bad = m_controller->board().conflicts();
    for (const auto& c : bad) {
        if (c.row < 9 && c.col < 9) m_conflict[c.row][c.col] = true;
    }
}

void GridView::setEditText(std::size_t r, std::size_t c, std::uint8_t v) {
    const std::size_t idx = r * 9 + c;
    if (idx >= m_edits.size()) return;
    if (v == 0) {
        SetWindowTextW(m_edits[idx], L"");
    } else {
        wchar_t s[2]{static_cast<wchar_t>(L'0' + v), 0};
        SetWindowTextW(m_edits[idx], s);
    }
}

void GridView::refreshFromModel() {
    if (!m_controller) return;
    m_syncing = true;

    for (std::size_t r = 0; r < 9; ++r) {
        for (std::size_t c = 0; c < 9; ++c) {
            const auto v = m_controller->board().get(r, c);
            setEditText(r, c, v);

            const bool given = m_controller->board().isGiven(r, c);
            SendMessageW(m_edits[r * 9 + c], EM_SETREADONLY, given ? TRUE : FALSE, 0);
        }
    }

    m_syncing = false;
    rebuildConflictMap();
    InvalidateRect(m_hwnd, nullptr, TRUE);
}

std::size_t GridView::indexFromHwnd(HWND edit) const {
    for (std::size_t i = 0; i < m_edits.size(); ++i) {
        if (m_edits[i] == edit) return i;
    }
    return m_edits.size();
}

bool GridView::cellFromId(int id, std::size_t& outR, std::size_t& outC) const {
    if (id < IdBase || id >= IdBase + 81) return false;
    const int idx = id - IdBase;
    outR = static_cast<std::size_t>(idx / 9);
    outC = static_cast<std::size_t>(idx % 9);
    return true;
}

} // namespace App

