#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    // Construct the login dialog.
    explicit LoginWindow(QWidget *parent = nullptr);
    // Clean up UI resources.
    ~LoginWindow();

private:
    Ui::LoginWindow *ui;

private slots:
    // Validate credentials and open the main window.
    void handleLogin();
    // Open the signup dialog.
    void openSignup();
};

#endif
