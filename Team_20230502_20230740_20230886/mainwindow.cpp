#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    loadTasks();
}

MainWindow::~MainWindow()
{
    delete ui;
}


QString filePath()
{
    return QStandardPaths::writableLocation(
               QStandardPaths::AppDataLocation)
           + "/tasks.txt";
}



void MainWindow::on_addBtn_clicked()
{
    QString task = ui->taskInput->text();
    if(task.isEmpty()) return;

    ui->taskList->addItem(task);
    ui->taskInput->clear();

    saveTasks();   // 👈 SAVE
}


void MainWindow::on_deleteBtn_clicked()
{
    delete ui->taskList->currentItem();
    saveTasks();   // 👈 SAVE
}



void MainWindow::on_doneBtn_clicked()
{
    QListWidgetItem *item = ui->taskList->currentItem();
    if(!item) return;

    QString text = item->text();

    if(text.startsWith("✔ "))
        item->setText(text.mid(2));
    else
        item->setText("✔ " + text);

    saveTasks();
}



void MainWindow::saveTasks()
{

    QDir().mkpath(QFileInfo(filePath()).path());

    QFile file(filePath());
    if(!file.open(QIODevice::WriteOnly)) return;

    QTextStream out(&file);

    for(int i=0;i<ui->taskList->count();i++)
        out << ui->taskList->item(i)->text() << "\n";

    file.close();
}



void MainWindow::loadTasks()
{
    QFile file(filePath());
    if(!file.open(QIODevice::ReadOnly)) return;

    QTextStream in(&file);

    while(!in.atEnd())
        ui->taskList->addItem(in.readLine());

    file.close();
}



void MainWindow::closeEvent(QCloseEvent *event)
{
    saveTasks();
    event->accept();
}


