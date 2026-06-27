#include "App/MainWindow.h"

#include <algorithm>

namespace App {

namespace {
constexpr wchar_t kMainClassName[] = L"SudokuMainWindow";
}

MainWindow::~MainWindow() {
    if (m_fontUi) {
        DeleteObject(m_fontUi);
        m_fontUi = nullptr;
    }
}

bool MainWindow::create(HINSTANCE hInst) {
    m_hInst = hInst;

    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = &MainWindow::WndProcThunk;
    wc.hInstance = hInst;
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    wc.lpszClassName = kMainClassName;
    if (!RegisterClassExW(&wc)) return false;

    m_hwnd = CreateWindowExW(
        0,
        kMainClassName,
        L"Sudoku (C++ OOP)",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        860,
        640,
        nullptr,
        nullptr,
        hInst,
        this);

    if (!m_hwnd) return false;

    ShowWindow(m_hwnd, SW_SHOW);
    UpdateWindow(m_hwnd);
    return true;
}

int MainWindow::run() {
    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK MainWindow::WndProcThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    MainWindow* self = reinterpret_cast<MainWindow*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    if (msg == WM_NCCREATE) {
        auto* cs = reinterpret_cast<CREATESTRUCTW*>(lParam);
        self = reinterpret_cast<MainWindow*>(cs->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
        if (self) self->m_hwnd = hwnd;
    }
    if (!self) return DefWindowProcW(hwnd, msg, wParam, lParam);
    return self->wndProc(hwnd, msg, wParam, lParam);
}

LRESULT MainWindow::wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            LOGFONTW lf{};
            lf.lfHeight = -16;
            lf.lfWeight = FW_NORMAL;
            lstrcpynW(lf.lfFaceName, L"Segoe UI", LF_FACESIZE);
            m_fontUi = CreateFontIndirectW(&lf);

            createChildControls();
            refreshUi();
            return 0;
        }

        case WM_SIZE: {
            layout(LOWORD(lParam), HIWORD(lParam));
            return 0;
        }

        case WM_COMMAND: {
            const int id = LOWORD(wParam);
            switch (id) {
                case BtnNew:
                    m_controller.newSamplePuzzle();
                    m_grid.refreshFromModel();
                    refreshUi();
                    return 0;
                case BtnClearUser:
                    m_controller.clearUserEntries();
                    m_grid.refreshFromModel();
                    refreshUi();
                    return 0;
                case BtnClearAll:
                    m_controller.clearAll();
                    m_grid.refreshFromModel();
                    refreshUi();
                    return 0;
                case BtnCheck:
                    m_controller.checkConflicts();
                    refreshUi();
                    return 0;
                case BtnSolve:
                    m_controller.solvePuzzle();
                    m_grid.refreshFromModel();
                    refreshUi();
                    return 0;
                default:
                    break;
            }
            return 0;
        }

        case WM_APP_BOARD_CHANGED: {
            // Grid already updated its visuals; we only refresh status text here.
            refreshUi();
            return 0;
        }

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            break;
    }
    return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void MainWindow::createChildControls() {
    // Grid view (positioned in layout()).
    m_grid.create(m_hwnd, m_hInst, 0, 0, 10, 10, &m_controller);

    auto mkBtn = [&](ControlId id, const wchar_t* text) -> HWND {
        HWND b = CreateWindowExW(
            0,
            L"BUTTON",
            text,
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            0,
            0,
            10,
            10,
            m_hwnd,
            reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
            m_hInst,
            nullptr);
        SendMessageW(b, WM_SETFONT, reinterpret_cast<WPARAM>(m_fontUi), TRUE);
        return b;
    };

    m_btnNew = mkBtn(BtnNew, L"New (sample)");
    m_btnClearUser = mkBtn(BtnClearUser, L"Clear user");
    m_btnClearAll = mkBtn(BtnClearAll, L"Clear all");
    m_btnCheck = mkBtn(BtnCheck, L"Check");
    m_btnSolve = mkBtn(BtnSolve, L"Solve");

    m_lblStatus = CreateWindowExW(
        0,
        L"STATIC",
        L"",
        WS_CHILD | WS_VISIBLE,
        0,
        0,
        10,
        10,
        m_hwnd,
        reinterpret_cast<HMENU>(static_cast<INT_PTR>(LblStatus)),
        m_hInst,
        nullptr);
    SendMessageW(m_lblStatus, WM_SETFONT, reinterpret_cast<WPARAM>(m_fontUi), TRUE);
}

void MainWindow::layout(int clientW, int clientH) {
    constexpr int pad = 14;
    constexpr int rightW = 220;
    constexpr int btnH = 34;
    constexpr int btnGap = 10;

    const int gridMaxW = std::max(0, clientW - rightW - 3 * pad);
    const int gridMaxH = std::max(0, clientH - 2 * pad);
    const int gridSize = std::max(200, std::min(gridMaxW, gridMaxH));

    const int gridX = pad;
    const int gridY = pad;

    SetWindowPos(m_grid.hwnd(), nullptr, gridX, gridY, gridSize, gridSize, SWP_NOZORDER | SWP_NOACTIVATE);

    const int paneX = gridX + gridSize + pad;
    int y = pad;
    SetWindowPos(m_btnNew, nullptr, paneX, y, rightW, btnH, SWP_NOZORDER | SWP_NOACTIVATE);
    y += btnH + btnGap;
    SetWindowPos(m_btnClearUser, nullptr, paneX, y, rightW, btnH, SWP_NOZORDER | SWP_NOACTIVATE);
    y += btnH + btnGap;
    SetWindowPos(m_btnClearAll, nullptr, paneX, y, rightW, btnH, SWP_NOZORDER | SWP_NOACTIVATE);
    y += btnH + btnGap;
    SetWindowPos(m_btnCheck, nullptr, paneX, y, rightW, btnH, SWP_NOZORDER | SWP_NOACTIVATE);
    y += btnH + btnGap;
    SetWindowPos(m_btnSolve, nullptr, paneX, y, rightW, btnH, SWP_NOZORDER | SWP_NOACTIVATE);

    y += btnH + btnGap;
    SetWindowPos(m_lblStatus, nullptr, paneX, y, rightW, 90, SWP_NOZORDER | SWP_NOACTIVATE);
}

void MainWindow::refreshUi() { setStatus(m_controller.statusText()); }

void MainWindow::setStatus(const std::wstring& text) {
    SetWindowTextW(m_lblStatus, text.empty() ? L" " : text.c_str());
}

} // namespace App

