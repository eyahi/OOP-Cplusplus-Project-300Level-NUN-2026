#ifndef PAUSEDIALOG_H
#define PAUSEDIALOG_H

#include <QDialog>

namespace Ui {
class PauseDialog;
}

class PauseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PauseDialog(QWidget *parent = nullptr);
    ~PauseDialog();

signals:
    void resumeClicked();
    void restartClicked();
    void menuClicked();
    void quitClicked();

private slots:
    void on_resumeButton_clicked();
    void on_restartButton_clicked();
    void on_menuButton_clicked();
    void on_quitButton_clicked();

private:
    Ui::PauseDialog *ui;
};

#endif // PAUSEDIALOG_H
