#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>


LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}


void LoginWindow::on_signupButton_clicked()
{
    SignupWindow signup;
    signup.exec();
}


void LoginWindow::on_enterButton_clicked()
{
    QString user = ui->usernameInput->text();
    QString pass = ui->passwordInput->text();

    if(user.isEmpty() || pass.isEmpty())
    {
        QMessageBox::warning(this,"Error","Fill all fields");
        return;
    }

    QFile file("users.txt");

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this,"Error","No accounts found");
        return;
    }

    QTextStream in(&file);
    bool found = false;

    while(!in.atEnd())
    {
        QString line = in.readLine();
        QStringList parts = line.split(":");

        if(parts[0] == user && parts[1] == pass)
        {
            found = true;
            break;
        }
    }

    file.close();

    if(found)
    {
        username = user;
        QMessageBox::information(this,"Success","Login successful!");
        accept();
    }
    else
    {
        QMessageBox::warning(this,"Error","Invalid login details");
    }
}






QString LoginWindow::getUsername() const
{
    return username;
}
QString LoginWindow::getPassword() const
{
    return password;
}
