#pragma once

#include "App/Controller.h"
#include "App/GridView.h"

#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>

namespace App {

class MainWindow {
public:
    MainWindow() = default;
    ~MainWindow();

    MainWindow(const MainWindow&) = delete;
    MainWindow& operator=(const MainWindow&) = delete;

    bool create(HINSTANCE hInst);
    int run();

private:
    static constexpr UINT WM_APP_BOARD_CHANGED = WM_APP + 1;

    enum ControlId : int {
        BtnNew = 2001,
        BtnClearUser = 2002,
        BtnClearAll = 2003,
        BtnCheck = 2004,
        BtnSolve = 2005,
        LblStatus = 2100,
    };

    static LRESULT CALLBACK WndProcThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT wndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

    void createChildControls();
    void layout(int clientW, int clientH);
    void refreshUi();
    void setStatus(const std::wstring& text);

    HINSTANCE m_hInst{nullptr};
    HWND m_hwnd{nullptr};

    HFONT m_fontUi{nullptr};

    HWND m_btnNew{nullptr};
    HWND m_btnClearUser{nullptr};
    HWND m_btnClearAll{nullptr};
    HWND m_btnCheck{nullptr};
    HWND m_btnSolve{nullptr};
    HWND m_lblStatus{nullptr};

    Controller m_controller{};
    GridView m_grid{};
};

} // namespace App

