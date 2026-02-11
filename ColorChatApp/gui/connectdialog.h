/*
 * File: connectdialog.h
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Header declarations for this component.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#pragma once
#include <QDialog>

class QLineEdit;
class QSpinBox;

class ConnectDialog : public QDialog {
    Q_OBJECT
public:
    explicit ConnectDialog(QWidget* parent = nullptr);

    QString host() const;
    quint16 port() const;

    QString nickname() const;
    QString about() const;

private:
    QLineEdit* m_host{nullptr};
    QSpinBox* m_port{nullptr};
    QLineEdit* m_nickname{nullptr};
    QLineEdit* m_about{nullptr};
};
