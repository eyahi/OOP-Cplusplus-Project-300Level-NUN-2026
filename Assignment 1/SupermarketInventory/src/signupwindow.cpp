#include "signupwindow.h"
#include "ui_signupwindow.h"
#include "userstore.h"
#include <QMessageBox>
#include <QLineEdit>

SignupWindow::SignupWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SignupWindow)
{
    // Basic UI wiring for signup.
    ui->setupUi(this);
    ui->usernameInput->setClearButtonEnabled(true);
    ui->passwordInput->setEchoMode(QLineEdit::Password);
    ui->confirmPasswordInput->setEchoMode(QLineEdit::Password);
    connect(ui->signupBtn, &QPushButton::clicked,
            this, &SignupWindow::handleSignup);

}

SignupWindow::~SignupWindow()
{
    // Clean up the UI.
    delete ui;
}


void SignupWindow::handleSignup()
{
    // Read user input.
    const QString u = ui->usernameInput->text().trimmed();
    const QString p = ui->passwordInput->text();
    const QString c = ui->confirmPasswordInput->text();

    // Basic input validation.
    if (u.isEmpty() || p.isEmpty() || c.isEmpty()) {
        QMessageBox::warning(this, "Error", "All fields are required.");
        return;
    }

    if (u.length() < 3) {
        QMessageBox::warning(this, "Error", "Username must be at least 3 characters.");
        return;
    }

    if (p.length() < 6) {
        QMessageBox::warning(this, "Error", "Password must be at least 6 characters.");
        return;
    }

    if (u.contains(",") || p.contains(",") || u.contains("\n") || p.contains("\n")) {
        QMessageBox::warning(this, "Error", "Commas and newlines are not allowed.");
        return;
    }

    // Confirm passwords match.
    if(p != c)
    {
        QMessageBox::warning(this,
                             "Error",
                             "Passwords do not match!");
        return;
    }

    // Load existing users if any.
    QVector<UserStore::UserRecord> records;
    QString pathUsed;
    QString loadError;
    if (!UserStore::loadUsersIfExists(&records, &loadError, &pathUsed)) {
        QMessageBox::warning(this, "Error", loadError.isEmpty()
                                               ? "Could not open users file."
                                               : loadError);
        return;
    }

    // Prevent duplicate usernames.
    if (UserStore::usernameExists(records, u)) {
        QMessageBox::warning(this, "Error", "Username already exists.");
        return;
    }

    // First user becomes admin if no admin exists yet.
    const bool hasAdmin = UserStore::anyAdmin(records);
    const UserStore::UserRecord newUser = UserStore::createUser(u, p, !hasAdmin);
    records.push_back(newUser);

    // Save new users file.
    QString writeError;
    if (!UserStore::saveUsers(records, &writeError)) {
        QMessageBox::warning(this, "Error", writeError.isEmpty()
                                               ? "Could not save user."
                                               : writeError);
        return;
    }

    QMessageBox::information(this,
                             "Success",
                             newUser.isAdmin
                                 ? "Account created! You are now an admin."
                                 : "Account created!");

    this->close();
}
