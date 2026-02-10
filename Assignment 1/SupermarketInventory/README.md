# Supermarket Inventory

Simple Qt Widgets app for managing a small supermarket inventory.

## Authors
- Blair Nwokike (20230050)
- Divine Udoh (20231712)
- Edeoga Chukwuemeka (20230941)

## What it does
- Login and signup with stored users.
- Admins can add, update, delete products.
- Normal users can view, search, and export reports.
- Low-stock items (qty <= 10) are highlighted.

## Roles
- First signup becomes **admin** automatically.
- All other signups are **normal users** unless you change the users file.

## Data storage (AppData)
Files are stored in:
`%APPDATA%\SupermarketInventory\SupermarketInventory`

- `users.csv`
- `inventory.csv`


## Project layout
```
SupermarketInventory/
  CMakeLists.txt
  README.md
  include/
    appdata.h
    loginwindow.h
    mainwindow.h
    signupwindow.h
    userstore.h
  src/
    appdata.cpp
    loginwindow.cpp
    main.cpp
    mainwindow.cpp
    signupwindow.cpp
    userstore.cpp
  ui/
    loginwindow.ui
    mainwindow.ui
    signupwindow.ui
```

Workspace root also contains:
- `CMakePresets.json`
- `.vscode/` settings

## Build & Run (VS Code + CMake Tools)
1) `CMake: Configure`
2) `CMake: Build`
3) `CMake: Run without Debugging`

If the app complains about missing Qt DLLs, run:
```
C:\Qt\6.10.1\mingw_64\bin\windeployqt.exe <path-to-exe>
```

## Notes
- Admin changes are saved on close and on logout.
- Inventory export writes a text report to a chosen location.
