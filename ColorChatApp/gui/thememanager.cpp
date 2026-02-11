/*
 * File: thememanager.cpp
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Implementation for this component.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#include "thememanager.h"

static QString darkMode() {
    return R"QSS(
        * { font-family: "Inter", "SF Pro Display", "Segoe UI", Arial; }
        /* Important: style QDialog too, otherwise dialogs stay white while text becomes light */
        QMainWindow, QDialog {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #0b1020, stop:0.5 #0f1633, stop:1 #130a1a);
        }
        QWidget { color: #EAF0FF; }
        QLabel { color: #EAF0FF; }

        /* Chips */
        QLabel[class="Chip"] {
            background: rgba(255,255,255,0.06);
            border: 1px solid rgba(255,255,255,0.12);
            padding: 6px 10px;
            border-radius: 12px;
            font-size: 12px;
            opacity: 0.92;
        }
        QLabel[class="Chip"][state="online"] {
            background: rgba(46, 204, 113, 0.18);
            border: 1px solid rgba(46, 204, 113, 0.45);
        }
        QLabel[class="Chip"][state="offline"] {
            background: rgba(148, 163, 184, 0.16);
            border: 1px solid rgba(148, 163, 184, 0.30);
        }

        /* Cards / panels */
        QWidget[class="Card"] {
            background: rgba(255,255,255,0.06);
            border: 1px solid rgba(255,255,255,0.12);
            border-radius: 18px;
        }

        /* Lists */
        QListWidget {
            background: transparent;
            border: none;
            outline: none;
        }
        QListWidget::item {
            padding: 10px 12px;
            margin: 6px 8px;
            border-radius: 14px;
            background: rgba(255,255,255,0.05);
        }
        QListWidget::item:selected {
            background: rgba(0,255,255,0.12);
            border: 1px solid rgba(0,255,255,0.25);
        }
        QListWidget::item:hover {
            background: rgba(255,255,255,0.08);
        }

        /* Inputs */
        QLineEdit, QTextEdit {
            background: rgba(255,255,255,0.06);
            border: 1px solid rgba(255,255,255,0.14);
            border-radius: 14px;
            padding: 10px 12px;
            selection-background-color: rgba(0,255,255,0.25);
        }
        QLineEdit:focus, QTextEdit:focus {
            border: 1px solid rgba(0,255,255,0.45);
        }

        /* Buttons */
        QPushButton, QToolButton {
            background: rgba(255,255,255,0.06);
            border: 1px solid rgba(255,255,255,0.14);
            border-radius: 14px;
            padding: 10px 12px;
        }
        QPushButton:hover, QToolButton:hover {
            background: rgba(0,255,255,0.10);
            border: 1px solid rgba(0,255,255,0.25);
        }
        QPushButton:pressed, QToolButton:pressed {
            background: rgba(255,0,255,0.10);
            border: 1px solid rgba(255,0,255,0.25);
        }

        /* Emoji grid buttons */
        QToolButton[class="Emoji"] {
            padding: 0px;
            font-size: 18px;
            min-width: 36px;
            min-height: 36px;
            border-radius: 10px;
        }

        QComboBox {
            background: rgba(255,255,255,0.06);
            border: 1px solid rgba(255,255,255,0.14);
            border-radius: 12px;
            padding: 6px 10px;
        }
        QComboBox::drop-down { border: none; padding-right: 8px; }
        QComboBox QAbstractItemView {
            background: #101831;
            border: 1px solid rgba(255,255,255,0.12);
            selection-background-color: rgba(0,255,255,0.22);
            outline: none;
        }

        QScrollArea { background: transparent; border: none; }
        QScrollBar:vertical {
            background: transparent;
            width: 12px;
            margin: 4px;
        }
        QScrollBar::handle:vertical {
            background: rgba(255,255,255,0.18);
            border-radius: 6px;
            min-height: 40px;
        }
        QScrollBar::handle:vertical:hover { background: rgba(0,255,255,0.25); }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }

        QMenu {
            background: #0f1633;
            border: 1px solid rgba(255,255,255,0.16);
            border-radius: 12px;
            padding: 8px;
        }
        QMenu::item {
            padding: 8px 12px;
            border-radius: 10px;
        }
        QMenu::item:selected { background: rgba(0,255,255,0.15); }
    )QSS";
}


static QString lightMode() {
    return R"QSS(
        * { font-family: "Inter", "SF Pro Display", "Segoe UI", Arial; }
        QMainWindow, QDialog {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #f6f9ff, stop:0.6 #f3f6ff, stop:1 #ffffff);
        }
        QWidget { color: #111827; }
        QLabel  { color: #111827; }

        /* Chips */
        QLabel[class="Chip"] {
            background: rgba(17,24,39,0.04);
            border: 1px solid rgba(17,24,39,0.10);
            padding: 6px 10px;
            border-radius: 12px;
            font-size: 12px;
            opacity: 0.95;
        }
        QLabel[class="Chip"][state="online"] {
            background: rgba(46, 204, 113, 0.16);
            border: 1px solid rgba(46, 204, 113, 0.40);
        }
        QLabel[class="Chip"][state="offline"] {
            background: rgba(148, 163, 184, 0.20);
            border: 1px solid rgba(148, 163, 184, 0.40);
        }

        /* Cards / panels */
        QWidget[class="Card"] {
            background: rgba(255,255,255,0.86);
            border: 1px solid rgba(17,24,39,0.10);
            border-radius: 18px;
        }

        /* Lists */
        QListWidget {
            background: transparent;
            border: none;
            outline: none;
        }
        QListWidget::item {
            padding: 10px 12px;
            margin: 6px 8px;
            border-radius: 14px;
            background: rgba(255,255,255,0.70);
            border: 1px solid rgba(17,24,39,0.06);
        }
        QListWidget::item:selected {
            background: rgba(0,180,255,0.14);
            border: 1px solid rgba(0,180,255,0.32);
        }
        QListWidget::item:hover {
            background: rgba(255,255,255,0.82);
        }

        /* Inputs */
        QLineEdit, QTextEdit {
            background: rgba(255,255,255,0.95);
            border: 1px solid rgba(17,24,39,0.14);
            border-radius: 14px;
            padding: 10px 12px;
            selection-background-color: rgba(0,180,255,0.22);
            selection-color: #0b1020;
        }
        QLineEdit:focus, QTextEdit:focus {
            border: 1px solid rgba(0,180,255,0.55);
        }

        /* Buttons */
        QPushButton, QToolButton {
            background: rgba(255,255,255,0.95);
            border: 1px solid rgba(17,24,39,0.14);
            border-radius: 14px;
            padding: 10px 12px;
        }
        QPushButton:hover, QToolButton:hover {
            background: rgba(0,180,255,0.08);
            border: 1px solid rgba(0,180,255,0.28);
        }
        QPushButton:pressed, QToolButton:pressed {
            background: rgba(255,0,255,0.08);
            border: 1px solid rgba(255,0,255,0.20);
        }

        /* Emoji grid buttons */
        QToolButton[class="Emoji"] {
            padding: 0px;
            font-size: 18px;
            min-width: 36px;
            min-height: 36px;
            border-radius: 10px;
        }

        QComboBox {
            background: rgba(255,255,255,0.95);
            border: 1px solid rgba(17,24,39,0.14);
            border-radius: 12px;
            padding: 6px 10px;
        }
        QComboBox::drop-down { border: none; padding-right: 8px; }
        QComboBox QAbstractItemView {
            background: #ffffff;
            border: 1px solid rgba(17,24,39,0.14);
            selection-background-color: rgba(0,180,255,0.18);
            outline: none;
        }

        QScrollArea { background: transparent; border: none; }
        QScrollBar:vertical {
            background: transparent;
            width: 12px;
            margin: 4px;
        }
        QScrollBar::handle:vertical {
            background: rgba(17,24,39,0.22);
            border-radius: 6px;
            min-height: 40px;
        }
        QScrollBar::handle:vertical:hover { background: rgba(0,180,255,0.28); }
        QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0px; }

        QMenu {
            background: #ffffff;
            color: #111827;
            border: 1px solid rgba(17,24,39,0.14);
            border-radius: 12px;
            padding: 8px;
        }
        QMenu::item {
            padding: 8px 12px;
            border-radius: 10px;
        }
        QMenu::item:selected { background: rgba(0,180,255,0.16); }
    )QSS";
}


QString ThemeManager::stylesheet(const QString& themeName) {
    const QString t = themeName.trimmed().toLower();
    if (t == "light") return lightMode();
    // Default
    return darkMode();
}
