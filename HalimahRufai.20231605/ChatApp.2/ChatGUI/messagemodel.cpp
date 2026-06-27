#include "messagemodel.h"

MessageModel::MessageModel(QObject *parent)
    : QAbstractListModel(parent)
{
    // OOP: Encapsulation & Abstraction
    // Setup TCP connection internally; QML doesn't see this detail
    socket.connectToHost("127.0.0.1", 1234);
    connect(&socket, &QTcpSocket::readyRead, this, &MessageModel::onReadyRead);

    // OOP: Object Instantiation
    // Initialize model with some sample messages
    m_messages.append({"YOU:Hello"});
    m_messages.append({"IN:Hi, how are you doing?"});
}

// OOP: Polymorphism
// Overrides virtual function from QAbstractListModel
int MessageModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_messages.size();
}

// OOP: Polymorphism
// Overrides virtual function from QAbstractListModel
// Provides different behavior for different roles
QVariant MessageModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_messages.size())
        return QVariant();

    const Message &msg = m_messages.at(index.row());

    if (role == Qt::DisplayRole || role == Qt::UserRole + 1)
        return msg.text; // QML accesses as model.display

    return QVariant();
}

// OOP: Polymorphism
// Overrides virtual function to provide custom role names
QHash<int, QByteArray> MessageModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::UserRole + 1] = "display"; // role used in QML
    return roles;
}

// OOP: Encapsulation
// Getter for currentMessage property
QString MessageModel::currentMessage() const
{
    return m_currentMessage;
}

// OOP: Encapsulation
// Setter for currentMessage property; notifies UI when changed
void MessageModel::setCurrentMessage(const QString &newCurrentMessage)
{
    if (m_currentMessage == newCurrentMessage)
        return;

    m_currentMessage = newCurrentMessage;
    emit currentMessageChanged(); // Observer pattern
}

// OOP: Abstraction & Encapsulation
// Called from QML via Q_INVOKABLE
// QML doesn't know how the message is sent or stored internally
void MessageModel::sendMessage()
{
    if (m_currentMessage.isEmpty())
        return;

    // Send to server (internal logic hidden)
    socket.write(m_currentMessage.toUtf8());

    // Add message to model (object instantiation)
    beginInsertRows(QModelIndex(), m_messages.size(), m_messages.size());
    m_messages.append({"YOU:" + m_currentMessage});
    endInsertRows();

    // Clear input using setter (encapsulation + observer)
    setCurrentMessage("");
}

// OOP: Encapsulation
// Handles incoming TCP data internally
// Called automatically by Qt signal-slot mechanism
void MessageModel::onReadyRead()
{
    QByteArray buffer = socket.readAll();
    QString newMessage = QString::fromUtf8(buffer);

    // Add incoming message to model (object instantiation)
    beginInsertRows(QModelIndex(), m_messages.size(), m_messages.size());
    m_messages.append({"IN:" + newMessage});
    endInsertRows();
}
