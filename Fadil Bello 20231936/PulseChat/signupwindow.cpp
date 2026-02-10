#include "signupwindow.h"
#include "ui_signupwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>


SignupWindow::SignupWindow(QWidget *parent)
    : QDialog(parent),
    ui(new Ui::SignupWindow)
{
    ui->setupUi(this);
}

SignupWindow::~SignupWindow()
{
    delete ui;
}

void SignupWindow::on_createAccountButton_clicked()
{
    QString user = ui->signupUsername->text();
    QString pass = ui->signupPassword->text();
    QString confirm = ui->confirmPassword->text();

    if(user.isEmpty() || pass.isEmpty())
    {
        QMessageBox::warning(this,"Error","Fill all fields");
        return;
    }

    if(pass != confirm)
    {
        QMessageBox::warning(this,"Error","Passwords do not match");
        return;
    }

    QFile file("users.txt");

    if(!file.open(QIODevice::Append | QIODevice::Text))
    {
        QMessageBox::critical(this,"Error","Cannot open file");
        return;
    }

    QTextStream out(&file);
    out << user << ":" << pass << "\n";
    file.close();

    QMessageBox::information(this,"Success","Account created!");
    accept();
}
