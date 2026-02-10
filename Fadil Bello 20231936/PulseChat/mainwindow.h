#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QTcpSocket>
#include <QtMultimedia/QSoundEffect>
#include <QTimer>
#include <QListWidgetItem>



extern int messageCounter;


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString &username, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void sendMessage();

private:
    Ui::MainWindow *ui;
    QString currentUsername;
    QTcpSocket *socket;
    QSoundEffect *sendSound;
    QSoundEffect *receiveSound;
    QString currentChatUser;

    bool isLoading;

    void saveMessage(const QString &msg);
    void addMyMessage(const QString &text);
    void loadMessages();
    void addOtherMessage(QString text);
    void setBackground();
    void animateMessage(QWidget *widget);
    void addSystemMessage(QString text);
    void addOnlineUser(QString user);
    void removeOnlineUser(QString user);
    void selectUser(QListWidgetItem *item);
    void loadMessagesForUser();
    void updateTick(QString id);








protected:
    void resizeEvent(QResizeEvent *event) override;

};

#endif // MAINWINDOW_H
