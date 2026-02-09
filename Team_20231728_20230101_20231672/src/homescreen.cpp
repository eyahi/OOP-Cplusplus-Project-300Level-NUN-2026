#include "homescreen.h"
#include "ui_homescreen.h"
#include <QApplication>

HomeScreen::HomeScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HomeScreen)
{
    ui->setupUi(this);
    showFullScreen();

    // Set button sizes
    ui->playerVsPlayerButton->setFixedSize(200, 40);
    ui->playerVsAIButton->setFixedSize(200, 40);
    ui->exitButton->setFixedSize(200, 40);

    // Center buttons using layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addStretch(); // Push buttons down
    mainLayout->addWidget(ui->playerVsPlayerButton, 0, Qt::AlignCenter);
    mainLayout->addWidget(ui->playerVsAIButton, 0, Qt::AlignCenter);
    mainLayout->addWidget(ui->exitButton, 0, Qt::AlignCenter);
    mainLayout->addStretch(); // Push buttons up

    // Set background image
    QString bgPath = "C:/Users/albaq/Documents/Chess/images/homebg.png";
    QString style = QString("QDialog { background-image: url(%1); background-position: center; background-repeat: no-repeat; background-size: cover; }").arg(bgPath);
    this->setStyleSheet(style);
}

HomeScreen::~HomeScreen()
{
    delete ui;
}

void HomeScreen::on_playerVsPlayerButton_clicked()
{
    emit playerVsPlayerSelected();
    accept();
}

void HomeScreen::on_playerVsAIButton_clicked()
{
    emit playerVsAISelected();
    accept();
}

void HomeScreen::on_exitButton_clicked()
{
    emit exitSelected();
    QApplication::quit();
}
