/*
 * File: thememanager.h
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Header declarations for this component.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#pragma once
#include <QString>

class ThemeManager {
public:
    static QString stylesheet(const QString& themeName);
};
