QT += core gui widgets
TARGET = ChessGame
TEMPLATE = app
CONFIG += c++11

# ========== SOURCE FILES ==========
SOURCES += \
    src/main.cpp \
    src/chesswidget.cpp \
    src/homescreen.cpp \
    src/mainwindow.cpp \
    src/pausedialog.cpp \
    src/Bishop.cpp \
    src/ChessBoard.cpp \
    src/King.cpp \
    src/Knight.cpp \
    src/Pawn.cpp \
    src/Piece.cpp \
    src/Queen.cpp \
    src/Rook.cpp

# ========== HEADER FILES ==========
HEADERS += \
    src/chesswidget.h \
    src/homescreen.h \
    src/mainwindow.h \
    src/pausedialog.h \
    src/Bishop.h \
    src/ChessBoard.h \
    src/King.h \
    src/Knight.h \
    src/Pawn.h \
    src/Piece.h \
    src/Queen.h \
    src/Rook.h

# ========== UI FILES ==========
# LIST EACH FILE EXPLICITLY - NO WILDCARDS!
FORMS += \
    ui/homescreen.ui \
    ui/mainwindow.ui \
    ui/pausedialog.ui

# ========== RESOURCES ==========
RESOURCES += \
    images/chess_resources.qrc

# ========== COMPILER ==========
QMAKE_CXXFLAGS += -Wall -Wextra

# Output directories
DESTDIR = build
OBJECTS_DIR = build/.obj
MOC_DIR = build/.moc
RCC_DIR = build/.rcc
UI_DIR = build/.ui
