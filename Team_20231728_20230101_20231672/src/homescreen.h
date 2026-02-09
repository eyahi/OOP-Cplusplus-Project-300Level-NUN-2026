#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <QDialog>

namespace Ui {
class HomeScreen;
}

class HomeScreen : public QDialog
{
    Q_OBJECT

public:
    explicit HomeScreen(QWidget *parent = nullptr);
    ~HomeScreen();
public:
    void setGameMode(int mode); // 0 = PvP, 1 = PvAI

private:
    int gameMode; // 0 = PvP, 1 = PvAI
signals:
    void playerVsPlayerSelected();
    void playerVsAISelected();
    void exitSelected();

private slots:
    void on_playerVsPlayerButton_clicked();
    void on_playerVsAIButton_clicked();
    void on_exitButton_clicked();

private:
    Ui::HomeScreen *ui;
};

#endif // HOMESCREEN_H
