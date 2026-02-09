/********************************************************************************
** Form generated from reading UI file 'homescreen.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_HOMESCREEN_H
#define UI_HOMESCREEN_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_HomeScreen
{
public:
    QVBoxLayout *verticalLayout_2;
    QPushButton *playerVsPlayerButton;
    QPushButton *playerVsAIButton;
    QPushButton *exitButton;

    void setupUi(QDialog *HomeScreen)
    {
        if (HomeScreen->objectName().isEmpty())
            HomeScreen->setObjectName("HomeScreen");
        HomeScreen->resize(504, 302);
        verticalLayout_2 = new QVBoxLayout(HomeScreen);
        verticalLayout_2->setObjectName("verticalLayout_2");
        playerVsPlayerButton = new QPushButton(HomeScreen);
        playerVsPlayerButton->setObjectName("playerVsPlayerButton");
        playerVsPlayerButton->setMinimumSize(QSize(250, 50));

        verticalLayout_2->addWidget(playerVsPlayerButton);

        playerVsAIButton = new QPushButton(HomeScreen);
        playerVsAIButton->setObjectName("playerVsAIButton");
        playerVsAIButton->setMinimumSize(QSize(250, 50));

        verticalLayout_2->addWidget(playerVsAIButton);

        exitButton = new QPushButton(HomeScreen);
        exitButton->setObjectName("exitButton");
        exitButton->setMinimumSize(QSize(250, 50));

        verticalLayout_2->addWidget(exitButton);


        retranslateUi(HomeScreen);

        QMetaObject::connectSlotsByName(HomeScreen);
    } // setupUi

    void retranslateUi(QDialog *HomeScreen)
    {
        HomeScreen->setWindowTitle(QCoreApplication::translate("HomeScreen", "Dialog", nullptr));
        playerVsPlayerButton->setText(QCoreApplication::translate("HomeScreen", "Player Vs Player", nullptr));
        playerVsAIButton->setText(QCoreApplication::translate("HomeScreen", "Player Vs AI", nullptr));
        exitButton->setText(QCoreApplication::translate("HomeScreen", "Exit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class HomeScreen: public Ui_HomeScreen {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_HOMESCREEN_H
