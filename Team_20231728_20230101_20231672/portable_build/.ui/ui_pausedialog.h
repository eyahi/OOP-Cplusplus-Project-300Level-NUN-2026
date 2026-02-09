/********************************************************************************
** Form generated from reading UI file 'pausedialog.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PAUSEDIALOG_H
#define UI_PAUSEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_PauseDialog
{
public:
    QVBoxLayout *verticalLayout;
    QPushButton *resumeButton;
    QPushButton *restartButton;
    QPushButton *menuButton;
    QPushButton *quitButton;

    void setupUi(QDialog *PauseDialog)
    {
        if (PauseDialog->objectName().isEmpty())
            PauseDialog->setObjectName("PauseDialog");
        PauseDialog->resize(400, 300);
        verticalLayout = new QVBoxLayout(PauseDialog);
        verticalLayout->setObjectName("verticalLayout");
        resumeButton = new QPushButton(PauseDialog);
        resumeButton->setObjectName("resumeButton");

        verticalLayout->addWidget(resumeButton);

        restartButton = new QPushButton(PauseDialog);
        restartButton->setObjectName("restartButton");

        verticalLayout->addWidget(restartButton);

        menuButton = new QPushButton(PauseDialog);
        menuButton->setObjectName("menuButton");

        verticalLayout->addWidget(menuButton);

        quitButton = new QPushButton(PauseDialog);
        quitButton->setObjectName("quitButton");

        verticalLayout->addWidget(quitButton);


        retranslateUi(PauseDialog);

        QMetaObject::connectSlotsByName(PauseDialog);
    } // setupUi

    void retranslateUi(QDialog *PauseDialog)
    {
        PauseDialog->setWindowTitle(QCoreApplication::translate("PauseDialog", "Dialog", nullptr));
        resumeButton->setText(QCoreApplication::translate("PauseDialog", "Resume Game", nullptr));
        restartButton->setText(QCoreApplication::translate("PauseDialog", "Restart Game", nullptr));
        menuButton->setText(QCoreApplication::translate("PauseDialog", "Back to Menu", nullptr));
        quitButton->setText(QCoreApplication::translate("PauseDialog", "Quit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class PauseDialog: public Ui_PauseDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PAUSEDIALOG_H
