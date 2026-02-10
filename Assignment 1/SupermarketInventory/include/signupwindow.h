#ifndef SIGNUPWINDOW_H
#define SIGNUPWINDOW_H


#include <QDialog>

namespace Ui {
class SignupWindow;
}

class SignupWindow : public QDialog
{
    Q_OBJECT

public:
    // Construct the signup dialog.
    explicit SignupWindow(QWidget *parent = nullptr);
    // Clean up UI resources.
    ~SignupWindow();

private:
    Ui::SignupWindow *ui;

private slots:
    // Validate input and create a new user.
    void handleSignup();
};

#endif
