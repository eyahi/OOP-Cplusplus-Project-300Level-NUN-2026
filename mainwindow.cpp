/*
  Names: Amira Dahiru Shettima, Roufiat ize saliu, rahma Sule habu
  course: object oriented programming
  filename: mainwindow.h
  Description: declaration of the mainwindow class for the to do manager
  date: january 2026
 */
#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QCheckBox>
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setStyleSheet(
        "QMainWindow{ background-color: #f5f7fa;}"
        );
    ui->addButton->setStyleSheet(

        "QPushButton {"

        "background-color: #4CAF50;"

        "color: white;"

        "border-radius: 8px;"

        "padding: 8px;"

        "font-weight: bold;"

        "}"

        "QPushButton:hover { background-color: #45a049; }"

        );

    ui->deleteButton->setStyleSheet(

        "QPushButton {"

        "background-color: #f44336;"

        "color: white;"

        "border-radius: 8px;"

        "padding: 8px;"

        "font-weight: bold;"

        "}"

        "QPushButton:hover { background-color: #da190b; }"

        );

    ui->taskList->setStyleSheet(

            "QListWidget {"
        "   background-color: #ffffff;"

        "   border: 1px solid #cccccc;"

        "   border-radius: 10px;"

        "}"

        "QListWidget::item {"

        "   background: transparent;" /* Let the taskWidget show through */

        "   border-bottom: 1px solid #f0f0f0;"

        "}"

        "QListWidget::item:selected {"

        "   background-color: #e8f4ff;"


            "}"

            );


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_addButton_clicked()
{
    QString taskText = ui->taskInput->text();
    QTime taskTime = ui->taskTime->time();
    if (taskText.isEmpty()) return;
    QListWidgetItem *item = new QListWidgetItem(ui->taskList);
    QWidget *taskWidget = new QWidget();
    QCheckBox *checkBox = new QCheckBox();
    QLabel *taskLabel = new QLabel(taskText);
    taskLabel->setStyleSheet("color: #2c3e50; font-weight: bold;");
    QLabel *timeLabel = new QLabel(taskTime.toString("hh:mm AP"));
    timeLabel->setStyleSheet("color: #7f8c8d;");
    timeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    QHBoxLayout *layout = new QHBoxLayout(taskWidget);
    layout->setContentsMargins(15, 10, 15, 10); // Big margins so it's not clipped
    layout->setSpacing(10);
    layout->addWidget(checkBox);
    layout->addWidget(taskLabel);
    layout->addStretch();
    layout->addWidget(timeLabel);
    taskWidget->setLayout(layout);
    item->setSizeHint(taskWidget->sizeHint());
    ui->taskList->addItem(item);
    ui->taskList->setItemWidget(item, taskWidget);
    ui->taskInput->clear();

 }


 void MainWindow::on_deleteButton_clicked()
 {

     int currentRow = ui->taskList->currentRow();

     if (currentRow >= 0) {

         QListWidgetItem *item = ui->taskList->takeItem(currentRow);

         delete item;

     }

 }


