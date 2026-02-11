/*
 * File: connectdialog.cpp
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Implementation for this component.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#include "connectdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDialogButtonBox>

ConnectDialog::ConnectDialog(QWidget* parent)
    : QDialog(parent) {
    setWindowTitle("Connect to Chat Server");
    setModal(true);
    setMinimumWidth(460);

    auto* root = new QVBoxLayout(this);
    root->setSpacing(12);

    auto mkRow = [&](const QString& label, QWidget* field){
        auto* row = new QHBoxLayout();
        row->setSpacing(10);
        auto* lab = new QLabel(label, this);
        lab->setMinimumWidth(120);
        row->addWidget(lab);
        row->addWidget(field, 1);
        root->addLayout(row);
    };

    m_host = new QLineEdit("127.0.0.1", this);
    m_port = new QSpinBox(this);
    m_port->setRange(1, 65535);
    m_port->setValue(8080);

    m_nickname = new QLineEdit(this);
    m_nickname->setPlaceholderText("Your nickname (e.g., Chigo)");

    m_about = new QLineEdit(this);
    m_about->setPlaceholderText("Short bio (optional)");


    mkRow("Host", m_host);
    mkRow("Port", m_port);
    mkRow("Nickname", m_nickname);
    mkRow("About", m_about);

    auto* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    root->addWidget(buttons);

    // slight "card" vibe
    setProperty("class", "Card");
    setObjectName("ConnectDialog");
    setStyleSheet(R"QSS(
        QDialog#ConnectDialog { border-radius: 18px; }
    )QSS");
}

QString ConnectDialog::host() const { return m_host->text().trimmed(); }
quint16 ConnectDialog::port() const { return static_cast<quint16>(m_port->value()); }
QString ConnectDialog::nickname() const { return m_nickname->text().trimmed(); }
QString ConnectDialog::about() const { return m_about->text().trimmed(); }
