#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H
#include "signupwindow.h"

#include <QDialog>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

    QString getUsername() const;
    QString getPassword() const;


private slots:
    void on_enterButton_clicked();
    void on_signupButton_clicked();


private:
    Ui::LoginWindow *ui; // Pointer to the generated UI class
    QString username;
    QString password;
};

#endif // LOGINWINDOW_H
