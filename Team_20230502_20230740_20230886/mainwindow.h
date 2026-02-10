#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QCloseEvent>

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addBtn_clicked();

    void on_deleteBtn_clicked();

    void on_doneBtn_clicked();

    void saveTasks();
    void loadTasks();
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H


