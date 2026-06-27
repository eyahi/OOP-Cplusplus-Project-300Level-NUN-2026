#include "pausedialog.h"
#include "ui_pausedialog.h"
#include <QApplication>

PauseDialog::PauseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PauseDialog)
{
    ui->setupUi(this);
    setWindowTitle("Game Paused");
    setFixedSize(300, 300);
}

PauseDialog::~PauseDialog()
{
    delete ui;
}

void PauseDialog::on_resumeButton_clicked()
{
    emit resumeClicked();
    accept();
}

void PauseDialog::on_restartButton_clicked()
{
    emit restartClicked();
    accept();
}

void PauseDialog::on_menuButton_clicked()
{
    emit menuClicked();
    accept();
}

void PauseDialog::on_quitButton_clicked()
{
    emit quitClicked();
    QApplication::quit();
}
