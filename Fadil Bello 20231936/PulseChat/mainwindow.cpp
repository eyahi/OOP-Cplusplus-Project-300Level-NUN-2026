#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTime>
#include <QLabel>
#include <QHBoxLayout>
#include <QListWidgetItem>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QHostAddress>
#include <QtMultimedia/QSoundEffect>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>


int messageCounter = 0;


MainWindow::MainWindow(const QString &username, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    currentUsername = username;


    isLoading = false;



    ui->setupUi(this);




    ui->chatList->setStyleSheet(
        "background: rgba(0,0,0,120);"
        "border-radius: 15px;"
        );

    ui->messageInput->setStyleSheet(
        "background: rgba(0,0,0,150);"
        "color: white;"
        "border-radius: 12px;"
        "padding: 6px;"
        "font-size: 12px;"
        );




    socket = new QTcpSocket(this);
    socket->connectToHost(" 10.112.64.42", 1234);

    socket->write(("LOGIN:" + currentUsername).toUtf8());

    connect(ui->onlineList, &QListWidget::itemClicked,
            this, &MainWindow::selectUser);





    sendSound = new QSoundEffect(this);
    sendSound->setSource(QUrl("qrc:/send.wav"));

    receiveSound = new QSoundEffect(this);
    receiveSound->setSource(QUrl("qrc:/recieve.wav"));



    connect(socket, &QTcpSocket::readyRead, this, [=]()
            {
                QByteArray data = socket->readAll();
                QString msg = QString::fromUtf8(data);

                QStringList messages = msg.split("\n", Qt::SkipEmptyParts);

                for(QString m : messages)
                {
                    // USER JOIN
                    if(m.startsWith("JOIN:"))
                    {
                        QString user = m.mid(5);
                        if(user != currentUsername)
                        {
                            addSystemMessage(user + " joined");
                            addOnlineUser(user);
                        }
                    }

                    // USER LEFT
                    else if(m.startsWith("LEFT:"))
                    {
                        QString user = m.mid(5);
                        addSystemMessage(user + " left");
                        removeOnlineUser(user);
                    }

                    // REAL MESSAGE
                    else if(m.startsWith("FROM:"))
                    {
                        QString sender = m.section("|",0,0).mid(5);
                        QString text   = m.section("|",2,2).mid(4);

                        addOtherMessage(sender + ": " + text);
                    }

                    // DELIVERY TICK
                    else if(m.startsWith("DELIVERED:"))
                    {
                        QString id = m.mid(10);
                        updateTick(id);
                    }
                }
            });



    loadMessages();
    qDebug() << "Logged in as:" << currentUsername;


    ui->welcomeLabel->setText("Welcome " + username);


    addMyMessage("Welcome to PulseChat 🔥");

    setWindowTitle("PulseChat");
    resize(480, 720);
    setMinimumSize(420, 640);

    connect(ui->sendButton, &QPushButton::clicked,
            this, &MainWindow::sendMessage);

    connect(ui->messageInput, &QLineEdit::returnPressed,
            this, &MainWindow::sendMessage);

    // Dark theme
    this->setStyleSheet(R"(
        ...






        QTextEdit {
            background-color: #1a1a2e;
            color: #ffffff;
            border: none;
            padding: 10px;
            font-size: 14px;
        }

        QLineEdit {
            background-color: #16213e;
            color: #ffffff;
            border-radius: 18px;
            padding: 10px;
            font-size: 14px;
        }

        QPushButton {
            background-color: #6a00ff;
            color: white;
            border-radius: 18px;
            padding: 8px 14px;
            font-size: 16px;
        }

        QPushButton:hover {
            background-color: #8a2be2;
        }
    )");

    setBackground();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::addMyMessage(const QString &text)
{
    QString time = QTime::currentTime().toString("hh:mm");

    QListWidgetItem *item = new QListWidgetItem();
    QWidget *bubble = new QWidget();

    QLabel *label = new QLabel(
        "<div style='font-size:11px; opacity:0.7;'>You • " + time + "</div>"
                                                                    "<div style='font-size:14px;'>" + text + "</div>"
        );
    label->setWordWrap(true);
    label->setTextFormat(Qt::RichText);
    label->setStyleSheet(
        "background:#6a00ff;"
        "color:white;"
        "padding:10px 14px;"
        "border-radius:18px;"
        );

    QHBoxLayout *layout = new QHBoxLayout(bubble);
    layout->addStretch();
    layout->addWidget(label);
    layout->setContentsMargins(10, 4, 10, 4);

    item->setSizeHint(bubble->sizeHint());
    ui->chatList->addItem(item);
    QTimer::singleShot(0, this, [=]() {
        animateMessage(bubble);
    });


    ui->chatList->setItemWidget(item, bubble);


    ui->chatList->scrollToBottom();
}

void MainWindow::saveMessage(const QString &msg)
{
    if(isLoading)
        return;

    if(currentChatUser.isEmpty())
        return;

    QString fileName =
        currentUsername + "_" + currentChatUser + ".txt";

    QFile file(fileName);

    if(file.open(QIODevice::Append | QIODevice::Text))
    {
        QTextStream out(&file);
        out << msg << "\n";
        file.close();
    }
}



void MainWindow::animateMessage(QWidget *widget)
{
    // Fade effect
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect(widget);
    widget->setGraphicsEffect(effect);

    QPropertyAnimation *fade = new QPropertyAnimation(effect, "opacity");
    fade->setDuration(300);
    fade->setStartValue(0.0);
    fade->setEndValue(1.0);
    fade->start(QAbstractAnimation::DeleteWhenStopped);

    // Slide effect
    QPoint startPos = widget->pos() + QPoint(50, 0);
    QPropertyAnimation *slide = new QPropertyAnimation(widget, "pos");
    slide->setDuration(300);
    slide->setStartValue(startPos);
    slide->setEndValue(widget->pos());
    slide->start(QAbstractAnimation::DeleteWhenStopped);
}

void MainWindow::selectUser(QListWidgetItem *item)
{
    QString name = item->text();

    name.remove("🟢 ");
    name.remove("⚪ ");

    currentChatUser = name;

    ui->chatList->clear();

    addSystemMessage("Now chatting with " + name);

    loadMessagesForUser();
}


void MainWindow::loadMessages()
{
    QString safeName = currentUsername.trimmed().toLower();
    QFile file(safeName + "_chat.txt");


    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);

        while (!in.atEnd())
        {
            QString line = in.readLine();
            addMyMessage(line);
        }

        file.close();
    }
}


void MainWindow::setBackground()
{
    QPixmap bg(":/bp.png");

    if(bg.isNull())
        qDebug() << "IMAGE NOT FOUND";

    bg = bg.scaled(this->size(),
                   Qt::IgnoreAspectRatio,
                   Qt::SmoothTransformation);

    QPalette pal;
    pal.setBrush(QPalette::Window, bg);
    this->setAutoFillBackground(true);
    this->setPalette(pal);

}

void MainWindow::sendMessage()
{
    QString text = ui->messageInput->text();

    if(text.isEmpty() || currentChatUser.isEmpty())
        return;

    sendSound->play();

    messageCounter++;

    QString msgId = QString::number(messageCounter);

    QString payload =
        "TO:" + currentChatUser +
        "|ID:" + msgId +
        "|MSG:" + text;

    socket->write((payload + "\n").toUtf8());

    // show message with ✔
    addMyMessage(text + " ✔");

    saveMessage("You:" + text + "|ID:" + msgId);

    ui->messageInput->clear();
}

void MainWindow::updateTick(QString id)
{
    for(int i=0;i<ui->chatList->count();i++)
    {
        QListWidgetItem *item = ui->chatList->item(i);
        QWidget *w = ui->chatList->itemWidget(item);

        QLabel *label = w->findChild<QLabel*>();

        if(label && label->text().contains("✔"))
        {
            QString t = label->text();

            if(!t.contains("✔✔"))
            {
                t.replace("✔","✔✔");
                label->setText(t);
                break;
            }
        }
    }
}





void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    setBackground();
}


void MainWindow::addSystemMessage(QString text)
{
    QListWidgetItem *item = new QListWidgetItem();
    QLabel *label = new QLabel(text);

    label->setAlignment(Qt::AlignCenter);
    label->setStyleSheet(
        "color: gray;"
        "font-size: 11px;"
        );

    item->setSizeHint(label->sizeHint());
    ui->chatList->addItem(item);
    ui->chatList->setItemWidget(item, label);
}

void MainWindow::addOnlineUser(QString user)
{
    QListWidgetItem *item = new QListWidgetItem("🟢 " + user);
    ui->onlineList->addItem(item);

    // auto select first user
    if(currentChatUser.isEmpty())
    {
        currentChatUser = user;
        addSystemMessage("Now chatting with " + user);
    }
}


void MainWindow::removeOnlineUser(QString user)
{
    for(int i = 0; i < ui->onlineList->count(); i++)
    {
        if(ui->onlineList->item(i)->text().contains(user))
        {
            ui->onlineList->item(i)->setText("⚪ " + user);
            break;
        }
    }
}

void MainWindow::loadMessagesForUser()
{
    isLoading = true;
    QString fileName =
        currentUsername + "_" + currentChatUser + ".txt";

    QFile file(fileName);

    if(file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);

        while(!in.atEnd())
        {
            QString line = in.readLine();

            if(line.startsWith("You:"))
                addMyMessage(line.mid(5));
            else
                addOtherMessage(line);
        }

        file.close();
    }

    isLoading = false;
}




void MainWindow::addOtherMessage(QString text)
{
    receiveSound->play();
    saveMessage(text);

    QString time = QTime::currentTime().toString("hh:mm");

    QListWidgetItem *item = new QListWidgetItem();
    QWidget *bubble = new QWidget();



    QLabel *label = new QLabel(
        "<div style='font-size:11px; opacity:0.6;'>Other • " + time + "</div>"
                                                                      "<div style='font-size:14px;'>" + text + "</div>"

        );

    label->setWordWrap(true);
    label->setTextFormat(Qt::RichText);
    label->setStyleSheet(
        "background:#db2777;"
        "color:white;"
        "padding:10px 14px;"
        "border-radius:18px;"
        );

    QHBoxLayout *layout = new QHBoxLayout(bubble);
    layout->addWidget(label);
    layout->addStretch();
    layout->setContentsMargins(10, 4, 10, 4);

    item->setSizeHint(bubble->sizeHint());
    ui->chatList->addItem(item);
    ui->chatList->setItemWidget(item, bubble);
    QTimer::singleShot(0, this, [=]() {
        animateMessage(bubble);
    });



    ui->chatList->scrollToBottom();
}




