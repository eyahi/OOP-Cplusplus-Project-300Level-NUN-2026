#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "userstore.h"
#include <QMessageBox>
#include "mainwindow.h"
#include "signupwindow.h"
//constructor
LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginWindow)
{
    // Basic UI wiring for login.
    ui->setupUi(this);
    ui->usernameInput->setClearButtonEnabled(true);//clear username
    connect(ui->loginBtn,
            &QPushButton::clicked,
            this,
            &LoginWindow::handleLogin);

    connect(ui->signupBtn,
            &QPushButton::clicked,
            this,
            &LoginWindow::openSignup);


}

//destructor
LoginWindow::~LoginWindow()
{
    // Clean up the UI.
    delete ui;
}

void LoginWindow::handleLogin()
{
    // Read user input.
    const QString u = ui->usernameInput->text().trimmed();
    const QString p = ui->passwordInput->text();

    // Validate inputs early.
    if (u.isEmpty() || p.isEmpty()) {
        QMessageBox::warning(this, "Error", "Username and password are required.");
        return;
    }

    // Load users from whichever file exists.
    QVector<UserStore::UserRecord> records;
    QString pathUsed;
    QString loadError;
    if (!UserStore::loadUsersIfExists(&records, &loadError, &pathUsed)) {
        QMessageBox::warning(this, "Error", loadError.isEmpty()
                                               ? "Could not open users file."
                                               : loadError);
        return;
    }

    if (pathUsed.isEmpty()) {
        QMessageBox::warning(this, "No Users",
                             "No users found. Please sign up first.");
        return;
    }

    // Verify credentials.
    bool isAdmin = false;
    const bool found = UserStore::verifyUser(records, u, p, &isAdmin); //checks if the username and password match

    if (found) {
        MainWindow *mw = new MainWindow(isAdmin);
        mw->setAttribute(Qt::WA_DeleteOnClose);
        mw->show();
        this->close();
    } else {
        // Invalid credentials feedback.
        QMessageBox::warning(this, "Error", "Invalid login!");
        ui->passwordInput->clear(); //reset password box
        ui->passwordInput->setFocus();//cursor goes to password box
    }
}

void LoginWindow::openSignup()
{
    // Open signup as a modal dialog and restore login when it closes.
    auto *s = new SignupWindow(this);
    s->setAttribute(Qt::WA_DeleteOnClose);
    s->setModal(true);
    connect(s, &QDialog::finished, this, [this]() {
        this->show();
        ui->passwordInput->clear();
        ui->passwordInput->setFocus();
    });
    this->hide();
    s->show();
}
