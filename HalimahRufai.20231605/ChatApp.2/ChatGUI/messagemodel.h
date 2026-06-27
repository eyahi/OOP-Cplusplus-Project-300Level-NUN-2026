#ifndef MESSAGEMODEL_H
#define MESSAGEMODEL_H

#include <QAbstractListModel>
#include <QTcpSocket>
#include <QVector>

// Structure to hold a single message
struct Message {
    QString text; // includes "YOU:" or "IN:" prefix
    // OOP: Encapsulation of a single message's data
};

class MessageModel : public QAbstractListModel
{
    Q_OBJECT
    /*
     * OOP: Encapsulation & Abstraction
     * Exposes 'currentMessage' to QML without revealing internal storage or network logic
     */
    Q_PROPERTY(QString currentMessage READ currentMessage WRITE setCurrentMessage NOTIFY currentMessageChanged)

public:
    explicit MessageModel(QObject *parent = nullptr);
    // OOP: Constructor / Object Instantiation

    // QAbstractListModel overrides
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    // OOP: Polymorphism - overrides virtual function from base class
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    // OOP: Polymorphism - provides different behavior for different roles
    QHash<int, QByteArray> roleNames() const override;
    // OOP: Polymorphism - maps role IDs to names

    // Input field text
    QString currentMessage() const;
    // OOP: Encapsulation - getter hides internal member
    void setCurrentMessage(const QString &newCurrentMessage);
    // OOP: Encapsulation - setter controls access to internal member

    // Called from QML to send a message
    Q_INVOKABLE void sendMessage();
    // OOP: Abstraction & Encapsulation - QML calls this without knowing network logic

signals:
    void currentMessageChanged();
    // OOP: Observer pattern - notifies UI when property changes

private slots:
    void onReadyRead(); // handle incoming messages
    // OOP: Encapsulation - internal slot handles TCP socket data

private:
    QVector<Message> m_messages; // stores all messages
    // OOP: Encapsulation of message list

    QString m_currentMessage;    // typed message in input
    // OOP: Encapsulation of current input

    QTcpSocket socket;           // TCP connection
    // OOP: Encapsulation of network communication
};

#endif // MESSAGEMODEL_H
