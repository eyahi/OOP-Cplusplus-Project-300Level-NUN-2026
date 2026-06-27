/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *taskInput;
    QPushButton *addBtn;
    QListWidget *taskList;
    QHBoxLayout *horizontalLayout;
    QPushButton *doneBtn;
    QPushButton *deleteBtn;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        taskInput = new QLineEdit(centralwidget);
        taskInput->setObjectName("taskInput");
        taskInput->setAlignment(Qt::AlignmentFlag::AlignCenter);

        verticalLayout->addWidget(taskInput);

        addBtn = new QPushButton(centralwidget);
        addBtn->setObjectName("addBtn");

        verticalLayout->addWidget(addBtn);

        taskList = new QListWidget(centralwidget);
        taskList->setObjectName("taskList");

        verticalLayout->addWidget(taskList);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        doneBtn = new QPushButton(centralwidget);
        doneBtn->setObjectName("doneBtn");

        horizontalLayout->addWidget(doneBtn);

        deleteBtn = new QPushButton(centralwidget);
        deleteBtn->setObjectName("deleteBtn");

        horizontalLayout->addWidget(deleteBtn);


        verticalLayout->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        taskInput->setText(QString());
        taskInput->setPlaceholderText(QCoreApplication::translate("MainWindow", "Add Task", nullptr));
        addBtn->setText(QCoreApplication::translate("MainWindow", "Add", nullptr));
        doneBtn->setText(QCoreApplication::translate("MainWindow", "Done", nullptr));
        deleteBtn->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
