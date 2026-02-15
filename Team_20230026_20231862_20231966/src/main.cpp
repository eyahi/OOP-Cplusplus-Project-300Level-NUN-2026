#include "App/MainWindow.h"

#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>

// MinGW (mingw.org) doesn't support -municode, so we use WinMain even though
// the rest of the app uses wide Win32 APIs (CreateWindowExW, etc.).
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    App::MainWindow win;
    if (!win.create(hInstance)) return 1;
    return win.run();
}

