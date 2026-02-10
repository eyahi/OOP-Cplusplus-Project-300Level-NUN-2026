#include "PasswordManager.h"
#include <QFile>
#include <QTextStream>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QInputDialog>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

#ifdef Q_OS_WIN
#include <windows.h>
#include <lmcons.h>
#endif

PasswordManager::PasswordManager(const QString& passwordFilePath)
    : m_passwordFile(passwordFilePath) {}

// load saved password hash from file
void PasswordManager::loadPassword() {
    QFile file(m_passwordFile);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        m_currentPasswordHash = in.readLine();
        file.close();
    }
}

// save password hash to file
void PasswordManager::savePassword(const QString& passwordHash) {
    m_currentPasswordHash = passwordHash;
    QFile file(m_passwordFile);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << passwordHash;
        file.close();
    }
}

void PasswordManager::removePassword() {
    m_currentPasswordHash.clear();
    QFile::remove(m_passwordFile);
}

bool PasswordManager::hasPassword() const {
    return !m_currentPasswordHash.isEmpty();
}

// hash password using SHA256
QString PasswordManager::hashPassword(const QString& password) const {
    return QString(QCryptographicHash::hash(
                       password.toUtf8(),
                       QCryptographicHash::Sha256).toHex());
}

// custom password dialog with forgot password option
bool PasswordManager::verifyPassword(QWidget* parent) {
    if (!hasPassword()) {
        return true; // no password set, let them through
    }

    QDialog dialog(parent);
    dialog.setWindowTitle("Password Required");
    dialog.setModal(true);

    QVBoxLayout* layout = new QVBoxLayout(&dialog);

    QLabel* label = new QLabel("Enter your diary password:");
    QLineEdit* passwordInput = new QLineEdit();
    passwordInput->setEchoMode(QLineEdit::Password);

    // button layout
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* okBtn = new QPushButton("OK");
    QPushButton* cancelBtn = new QPushButton("Cancel");
    QPushButton* forgotBtn = new QPushButton("Forgot Password");

    okBtn->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; padding: 8px; }");
    cancelBtn->setStyleSheet(
        "QPushButton { background-color: #95a5a6; color: white; padding: 8px; }");
    forgotBtn->setStyleSheet(
        "QPushButton { background-color: #e67e22; color: white; padding: 8px; }");

    buttonLayout->addWidget(okBtn);
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addWidget(forgotBtn);

    layout->addWidget(label);
    layout->addWidget(passwordInput);
    layout->addLayout(buttonLayout);

    bool passwordVerified = false;

    // check if password is correct
    QObject::connect(okBtn, &QPushButton::clicked, [&]() {
        if (hashPassword(passwordInput->text()) == m_currentPasswordHash) {
            passwordVerified = true;
            dialog.accept();
        } else {
            QMessageBox::warning(&dialog, "Incorrect", "Wrong password!");
            passwordInput->clear();
        }
    });

    QObject::connect(cancelBtn, &QPushButton::clicked,
                     &dialog, &QDialog::reject);

    // forgot password goes to recovery flow
    QObject::connect(forgotBtn, &QPushButton::clicked, [&]() {
        dialog.accept();
        forgotPasswordRecovery(parent);
    });

    // pressing enter clicks OK button
    QObject::connect(passwordInput, &QLineEdit::returnPressed,
                     okBtn, &QPushButton::click);

    dialog.exec();
    return passwordVerified;
}

bool PasswordManager::changePassword(QWidget* parent) {
    // verify current password first if one exists
    if (hasPassword()) {
        if (!verifyPassword(parent)) {
            return false;
        }
    }

    // get new password from user
    bool ok;
    QString newPassword = QInputDialog::getText(
        parent, "Set Password",
        "Enter new password:",
        QLineEdit::Password, "", &ok);

    if (ok && !newPassword.isEmpty()) {
        savePassword(hashPassword(newPassword));
        QMessageBox::information(parent, "Success",
                                 "Password set successfully!");
        return true;
    }
    return false;
}

// authenticate using Windows login password (Windows only)
bool PasswordManager::authenticateWithWindowsPassword(const QString& password) {
#ifdef Q_OS_WIN
    // get current username
    wchar_t username[UNLEN + 1];
    DWORD usernameLen = UNLEN + 1;
    GetUserNameW(username, &usernameLen);

    // get computer name
    wchar_t computerName[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD computerNameLen = MAX_COMPUTERNAME_LENGTH + 1;
    GetComputerNameW(computerName, &computerNameLen);

    std::wstring wPassword = password.toStdWString();

    // try to log in with Windows credentials
    HANDLE token;
    bool result = LogonUserW(
        username,
        computerName,
        wPassword.c_str(),
        LOGON32_LOGON_INTERACTIVE,
        LOGON32_PROVIDER_DEFAULT,
        &token
        );

    if (result) {
        CloseHandle(token);
        return true;
    }
    return false;
#else
    // not on Windows, can't use this feature
    QMessageBox::information(
        nullptr, "Not Supported",
        "Windows password authentication is only available on Windows.\n"
        "On other systems, please manually delete the password file.");
    return false;
#endif
}

// password recovery using Windows authentication
bool PasswordManager::forgotPasswordRecovery(QWidget* parent) {
    if (!hasPassword()) {
        QMessageBox::information(parent, "No Password",
                                 "There is no password set for this diary.");
        return false;
    }

#ifdef Q_OS_WIN
    QMessageBox::information(parent, "Windows Authentication",
                             "To recover your diary password, please enter your Windows login password.");

    bool ok;
    QString windowsPassword = QInputDialog::getText(
        parent, "Windows Password",
        "Enter your Windows login password:",
        QLineEdit::Password, "", &ok);

    if (!ok || windowsPassword.isEmpty()) {
        return false;
    }

    // verify Windows password
    if (authenticateWithWindowsPassword(windowsPassword)) {
        QMessageBox::information(parent, "Authentication Successful",
                                 "Windows authentication successful! You can now set a new diary password.");

        // let them set a new diary password
        QString newPassword = QInputDialog::getText(
            parent, "Reset Password",
            "Enter your new diary password:",
            QLineEdit::Password, "", &ok);

        if (ok && !newPassword.isEmpty()) {
            savePassword(hashPassword(newPassword));
            QMessageBox::information(parent, "Success",
                                     "Your diary password has been reset successfully!");
            return true;
        }
    } else {
        QMessageBox::critical(parent, "Authentication Failed",
                              "Incorrect Windows password!\n\n"
                              "Please make sure you're entering your Windows login password correctly.");
    }
#else
    // not on Windows, tell them to delete the file manually
    QMessageBox::information(parent, "Not Available",
                             "Windows password authentication is only available on Windows.\n\n"
                             "Please manually delete the password file:\n" + m_passwordFile);
#endif
    return false;
}
