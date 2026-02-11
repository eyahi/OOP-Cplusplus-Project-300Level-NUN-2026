/*
 * File: mainwindow.cpp
 * Authors: Chigo Daniel, Abdulrahman
 * Date: 2026-01-16
 * Purpose: Implements the main GUI window and user interactions.
 * Course/Assignment: C++ Project (Qt Chat Application)
 */

#include "mainwindow.h"
#include "thememanager.h"
#include "bubblewidget.h"
#include "connectdialog.h"

#include <QApplication>
#include <QStatusBar>
#include <QStyle>
#include <QWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QToolButton>
#include <QComboBox>
#include <QScrollArea>
#include <QScrollBar>
#include <QMessageBox>
#include <QTimer>
#include <QEvent>

#include <algorithm>

static QLabel* makeChip(const QString& text) {
    auto* lab = new QLabel(text);
    // Styling comes from ThemeManager via QLabel[class="Chip"] rules.
    lab->setProperty("class", "Chip");
    return lab;
}

static void setChipState(QLabel* chip, const QString& state) {
    if (!chip) return;
    chip->setProperty("state", state);
    // Force Qt to re-evaluate stylesheet selectors based on dynamic properties.
    chip->style()->unpolish(chip);
    chip->style()->polish(chip);
    chip->update();
}

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {

    m_client = new ChatClient(this);

    buildUi();
    startConnectFlow();

    connect(m_client, &ChatClient::connected, this, &MainWindow::onConnected);
    connect(m_client, &ChatClient::disconnected, this, &MainWindow::onDisconnected);
    connect(m_client, &ChatClient::connectionError, this, &MainWindow::onConnectionError);
    connect(m_client, &ChatClient::registered, this, &MainWindow::onRegistered);
    connect(m_client, &ChatClient::messageReceived, this, &MainWindow::onMessageReceived);
    connect(m_client, &ChatClient::roomJoined, this, &MainWindow::onRoomJoined);
    connect(m_client, &ChatClient::roomLeft, this, &MainWindow::onRoomLeft);
    connect(m_client, &ChatClient::roomCreated, this, &MainWindow::onRoomCreated);

    connect(m_conversations, &QListWidget::itemSelectionChanged, this, &MainWindow::onConversationChanged);
    connect(m_input, &QLineEdit::returnPressed, this, &MainWindow::sendCurrentMessage);
    connect(m_sendBtn, &QToolButton::clicked, this, &MainWindow::sendCurrentMessage);
    connect(m_theme, &QComboBox::currentTextChanged, this, &MainWindow::onThemeChanged);
    connect(m_dmBtn, &QToolButton::clicked, this, &MainWindow::startDmWithSelection);
    connect(m_roomBtn, &QToolButton::clicked, this, &MainWindow::startRoomWithSelection);
    connect(m_users, &QListWidget::itemDoubleClicked, this, &MainWindow::onUserDoubleClicked);
    connect(m_joinRoomBtn, &QToolButton::clicked, this, &MainWindow::joinRoomFromInput);
    connect(m_joinRoomInput, &QLineEdit::returnPressed, this, &MainWindow::joinRoomFromInput);
    connect(m_roomNameInput, &QLineEdit::returnPressed, this, &MainWindow::startRoomWithSelection);

    ensureRoomExists(m_globalRoomId, "🌍 Global");
    selectRoom(m_globalRoomId);
}

void MainWindow::buildUi() {
    setWindowTitle("ColorChat — Colorful GUI Client");
    resize(1200, 760);

    auto* splitter = new QSplitter(this);
    splitter->setHandleWidth(10);

    setupSidebar(splitter);
    setupChatArea(splitter);

    setCentralWidget(splitter);

    // default theme
    m_theme->setCurrentText("Dark");
    applyTheme("Dark");
}

void MainWindow::setupSidebar(QSplitter* splitter) {
    auto* sidebar = new QWidget(splitter);
    auto* side = new QVBoxLayout(sidebar);
    side->setContentsMargins(14, 14, 14, 14);
    side->setSpacing(12);

    auto* profileCard = new QWidget(sidebar);
    profileCard->setProperty("class", "Card");
        auto* pc = new QVBoxLayout(profileCard);
    pc->setContentsMargins(14, 14, 14, 14);
    pc->setSpacing(8);

    m_profileLine = new QLabel("Not connected", profileCard);
    m_profileLine->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_profileLine->setStyleSheet("font-weight: 700; font-size: 14px;");
    pc->addWidget(m_profileLine);

    auto* chips = new QHBoxLayout();
    chips->setSpacing(8);
    chips->addWidget(makeChip("🔒 Demo encryption"));
    m_connChip = makeChip("⚪ Offline");
    setChipState(m_connChip, "offline");
    chips->addWidget(m_connChip);
    chips->addStretch(1);
    pc->addLayout(chips);

    m_theme = new QComboBox(profileCard);
    // Only 2 modes as requested.
    m_theme->addItems({"Dark", "Light"});
    pc->addWidget(m_theme);

    side->addWidget(profileCard);

    auto* convLabel = new QLabel("Conversations", sidebar);
    convLabel->setStyleSheet("font-weight: 800; opacity: 0.9;");
    side->addWidget(convLabel);

    m_conversations = new QListWidget(sidebar);
    m_conversations->setMinimumWidth(280);
    side->addWidget(m_conversations, 1);

    auto* usersLabel = new QLabel("Online users", sidebar);
    usersLabel->setStyleSheet("font-weight: 800; opacity: 0.9;");
    side->addWidget(usersLabel);

    m_users = new QListWidget(sidebar);
    m_users->setMinimumHeight(180);
    m_users->setSelectionMode(QAbstractItemView::ExtendedSelection);
    side->addWidget(m_users);

    auto* usersHint = new QLabel("Select users. Double‑click to DM.", sidebar);
    usersHint->setStyleSheet("opacity:0.75; font-size: 12px; padding-left: 6px;");
    side->addWidget(usersHint);

    auto* userBtns = new QHBoxLayout();
    userBtns->setSpacing(10);
    m_dmBtn = new QToolButton(sidebar);
    m_dmBtn->setText("🔒 DM");
    m_dmBtn->setToolTip("Start a private message with 1 selected user");
    m_roomBtn = new QToolButton(sidebar);
    m_roomBtn->setText("👥 Create room");
    m_roomBtn->setToolTip("Create a group room with 2+ selected users");
    userBtns->addWidget(m_dmBtn);
    userBtns->addWidget(m_roomBtn);
    side->addLayout(userBtns);

    auto* roomCard = new QWidget(sidebar);
    roomCard->setProperty("class", "Card");
    auto* rc = new QVBoxLayout(roomCard);
    rc->setContentsMargins(14, 12, 14, 12);
    rc->setSpacing(8);

    auto* roomHint = new QLabel("Rooms & group chats", roomCard);
    roomHint->setStyleSheet("font-weight:800; opacity:0.9;");
    rc->addWidget(roomHint);

    m_roomNameInput = new QLineEdit(roomCard);
    m_roomNameInput->setPlaceholderText("Room name (optional)");
    rc->addWidget(m_roomNameInput);

    auto* row = new QHBoxLayout();
    row->setSpacing(10);
    m_joinRoomInput = new QLineEdit(roomCard);
    m_joinRoomInput->setPlaceholderText("Join room by ID…");
    m_joinRoomBtn = new QToolButton(roomCard);
    m_joinRoomBtn->setText("↪ Join");
    row->addWidget(m_joinRoomInput, 1);
    row->addWidget(m_joinRoomBtn);
    rc->addLayout(row);

    auto* small = new QLabel("Tip: Select 2+ users → Create room. Or create an empty room (no selection) and share its ID.", roomCard);
    small->setStyleSheet("opacity:0.75; font-size:12px;");
    small->setWordWrap(true);
    rc->addWidget(small);

    side->addWidget(roomCard);
    
    splitter->addWidget(sidebar);
    splitter->setStretchFactor(0, 0);
}

void MainWindow::setupChatArea(QSplitter* splitter) {
    auto* main = new QWidget(splitter);
    auto* root = new QVBoxLayout(main);
    root->setContentsMargins(14, 14, 14, 14);
    root->setSpacing(12);

    auto* header = new QWidget(main);
    header->setProperty("class", "Card");
        auto* h = new QHBoxLayout(header);
    h->setContentsMargins(14, 12, 14, 12);
    h->setSpacing(10);

    m_title = new QLabel("🌍 Global", header);
    m_title->setStyleSheet("font-size: 18px; font-weight: 900;");
    h->addWidget(m_title);
    h->addStretch(1);

    auto* reconnect = new QToolButton(header);
    reconnect->setText("🔌 Connect");
    connect(reconnect, &QToolButton::clicked, this, &MainWindow::startConnectFlow);
    h->addWidget(reconnect);

    root->addWidget(header);

    m_scroll = new QScrollArea(main);
    m_scroll->setWidgetResizable(true);

    m_messagesContainer = new QWidget(m_scroll);
    m_messagesLayout = new QVBoxLayout(m_messagesContainer);
    m_messagesLayout->setContentsMargins(12, 12, 12, 12);
    m_messagesLayout->setSpacing(6);
    m_messagesLayout->addStretch(1);
    m_scroll->setWidget(m_messagesContainer);
    m_scroll->viewport()->installEventFilter(this);

    updateWallpaper();

    root->addWidget(m_scroll, 1);

    setupEmojiPanel(root);
    setupComposer(root);

    splitter->addWidget(main);
    splitter->setStretchFactor(1, 1);
}

void MainWindow::setupEmojiPanel(QVBoxLayout* root) {
    m_emojiPanel = new QWidget(this);
    m_emojiPanel->setProperty("class", "Card");
    auto* ep = new QGridLayout(m_emojiPanel);
    ep->setContentsMargins(12, 10, 12, 10);
    ep->setHorizontalSpacing(8);
    ep->setVerticalSpacing(8);

    const QStringList emojis = {
        "😀","😁","😂","🤣","😍","😘","😎","🥳","😭","😅",
        "🤔","😴","👍","👎","🙏","👏","🤝","🎉","🔥","💯",
        "✅","❌","❤️","✨","⭐","🌟","🎁","🎈","🥂","🍀"
    };
    const int cols = 10;
    for (int i = 0; i < emojis.size(); ++i) {
        const QString e = emojis[i];
        auto* b = new QToolButton(m_emojiPanel);
        b->setProperty("class", "Emoji");
        b->setText(e);
        b->setToolTip(e);
        b->setCursor(Qt::PointingHandCursor);
        b->setFixedSize(36, 36);
        connect(b, &QToolButton::clicked, this, [this, e]() {
            insertEmoji(e);
            if (m_emojiPanel) m_emojiPanel->setVisible(false);
        });
        ep->addWidget(b, i / cols, i % cols);
    }
    m_emojiPanel->setVisible(false);
    root->addWidget(m_emojiPanel);
}

void MainWindow::setupComposer(QVBoxLayout* root) {
    auto* composer = new QWidget(this);
    composer->setProperty("class", "Card");
        auto* c = new QHBoxLayout(composer);
    c->setContentsMargins(14, 12, 14, 12);
    c->setSpacing(10);

    m_emojiBtn = new QToolButton(composer);
    m_emojiBtn->setText(QString::fromUtf8("😊"));
    m_emojiBtn->setToolTip("Emojis");
    m_emojiBtn->setCursor(Qt::PointingHandCursor);
    connect(m_emojiBtn, &QToolButton::clicked, this, &MainWindow::toggleEmojiPanel);

    m_input = new QLineEdit(composer);
    m_input->setPlaceholderText("Type a message… (Press Enter to send)");
    m_input->setClearButtonEnabled(true);

    m_sendBtn = new QToolButton(composer);
    m_sendBtn->setText("🚀 Send");

    c->addWidget(m_emojiBtn);
    c->addWidget(m_input, 1);
    c->addWidget(m_sendBtn);

    root->addWidget(composer);
}

void MainWindow::startConnectFlow() {
    ConnectDialog dlg(this);
    // keep current theme selection
    dlg.exec();

    if (dlg.result() != QDialog::Accepted) return;

    applyTheme(m_theme ? m_theme->currentText() : QString("Dark"));

    const QString nick = dlg.nickname().isEmpty() ? "Guest" : dlg.nickname();
    const QString about = dlg.about();

    // Keep profile simple for demos.
    m_client->profile()->setNickname(nick);
    m_client->profile()->setAbout(about);
    m_client->profile()->setStatus("Online");
    m_client->profile()->setStatusColor("#2ecc71");

    m_profileLine->setText(QString("%1  —  Online").arg(nick));

    m_client->connectToServer(dlg.host(), dlg.port());
}

void MainWindow::onConnected() {
    if (m_connChip) {
        m_connChip->setText("🟢 Online");
        setChipState(m_connChip, "online");
    }
    statusBar()->showMessage("Connected. Registering…");
    const QString nick = m_client->profile()->nickname().isEmpty() ? "Guest" : m_client->profile()->nickname();
    m_client->registerUser(nick);
}

void MainWindow::onDisconnected() {
    if (m_connChip) {
        m_connChip->setText("⚪ Offline");
        setChipState(m_connChip, "offline");
    }
    statusBar()->showMessage("Disconnected");
}

void MainWindow::onConnectionError(const QString& error) {
    QMessageBox::warning(this, "Connection error", error);
    if (m_connChip) {
        m_connChip->setText("⚪ Offline");
        setChipState(m_connChip, "offline");
    }
    statusBar()->showMessage("Connection error");
}

void MainWindow::onRegistered(const QString& userId, const QString& globalRoomId) {
    m_userId = userId;
    m_globalRoomId = globalRoomId.isEmpty() ? "global" : globalRoomId;
    ensureRoomExists(m_globalRoomId, "🌍 Global");
    selectRoom(m_globalRoomId);

    // Reset and show at least *you* immediately.
    m_onlineUsers.clear();
    if (!m_client->profile()->nickname().isEmpty()) m_onlineUsers[m_userId] = m_client->profile()->nickname();
    refreshOnlineUsersList();

    statusBar()->showMessage(QString("Registered as %1").arg(m_client->profile()->nickname()));
    m_profileLine->setText(QString("%1  —  %2\nID: %3")
        .arg(m_client->profile()->nickname(),
             m_client->profile()->status(),
             m_userId));
}

void MainWindow::ensureRoomExists(const QString& roomId, const QString& title) {
    if (m_roomItem.contains(roomId)) return;
    auto* item = new QListWidgetItem(title);
    item->setData(Qt::UserRole, roomId);
    m_conversations->addItem(item);
    m_roomItem[roomId] = item;
}

void MainWindow::selectRoom(const QString& roomId) {
    m_currentRoomId = roomId;
    if (m_roomItem.contains(roomId)) {
        m_conversations->setCurrentItem(m_roomItem[roomId]);
        setSidebarGlow(m_roomItem[roomId], false);
    }
    renderRoom(roomId);
}

void MainWindow::onConversationChanged() {
    auto* item = m_conversations->currentItem();
    if (!item) return;
    const QString roomId = item->data(Qt::UserRole).toString();
    selectRoom(roomId);
}

void MainWindow::clearLayout(QVBoxLayout* layout) {
    while (QLayoutItem* child = layout->takeAt(0)) {
        if (QWidget* w = child->widget()) w->deleteLater();
        if (QLayout* l = child->layout()) delete l;
        delete child;
    }
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event) {
    if (m_scroll && watched == m_scroll->viewport() && event->type() == QEvent::Resize) {
        updateAllBubbleWidths();
    }
    return QMainWindow::eventFilter(watched, event);
}

int MainWindow::computeMaxBubbleWidth() const {
    if (!m_scroll) return 900;
    int w = m_scroll->viewport()->width();
    // Use most of the chat width so medium messages stay on one line.
    int maxW = int(w * 0.98) - 24;
    if (maxW < 260) maxW = 260;
    return maxW;
}

void MainWindow::updateAllBubbleWidths() {
    if (!m_messagesContainer) return;
    const int maxW = computeMaxBubbleWidth();
    const auto bubbles = m_messagesContainer->findChildren<BubbleWidget*>();
    for (auto* b : bubbles) {
        b->setMaxBubbleWidth(maxW);
    }
}

void MainWindow::renderRoom(const QString& roomId) {
    // reset view
    clearLayout(m_messagesLayout);

    m_lastRenderedDate[roomId] = QDate();

    // add messages
    const auto& list = m_history[roomId];
    for (const auto& um : list) {
        appendMessageToView(roomId, um.msg, um.mine);
    }

    // spacer at bottom
    m_messagesLayout->addStretch(1);

    // title
    if (m_roomItem.contains(roomId)) m_title->setText(m_roomItem[roomId]->text());

    // keep bubbles wide as the window changes
    updateAllBubbleWidths();

    // scroll to bottom
    QTimer::singleShot(0, this, [this]{
        m_scroll->verticalScrollBar()->setValue(m_scroll->verticalScrollBar()->maximum());
    });
}

void MainWindow::appendMessageToView(const QString& roomId, const ChatMessage& message, bool mine) {
    // Date grouping headers
    const QDate d = message.timestamp().date();
    if (!m_lastRenderedDate.contains(roomId) || m_lastRenderedDate[roomId] != d) {
        m_lastRenderedDate[roomId] = d;
        auto* sep = new QLabel(d == QDate::currentDate() ? "Today" : d.toString("ddd, dd MMM yyyy"), m_messagesContainer);
        sep->setAlignment(Qt::AlignCenter);
        sep->setStyleSheet("opacity:0.7; font-weight:800; padding:10px 0;");
        m_messagesLayout->addWidget(sep);
    }

    const bool isLightTheme = m_isLightTheme;
    auto* bubble = new BubbleWidget(
        mine ? "You" : message.senderName(),
        message.content(),
        message.timestamp(),
        mine,
        QString(),
        isLightTheme,
        m_messagesContainer
    );
    bubble->setMessageId(message.id());
    bubble->setMaxBubbleWidth(computeMaxBubbleWidth());

    m_messagesLayout->addWidget(bubble);

    if (roomId == m_currentRoomId) {
        QTimer::singleShot(0, this, [this]{
            m_scroll->verticalScrollBar()->setValue(m_scroll->verticalScrollBar()->maximum());
        });
    }
}

void MainWindow::onMessageReceived(const QString& roomId, const ChatMessage& message, bool isMine) {
    m_history[roomId].append(UiMessage{message, isMine});

    ensureRoomExists(roomId, roomTitleForId(roomId));

    if (roomId == m_currentRoomId) {
        // remove bottom stretch, append, add stretch again
        if (m_messagesLayout->count() > 0) {
            QLayoutItem* last = m_messagesLayout->takeAt(m_messagesLayout->count() - 1);
            delete last;
        }
        appendMessageToView(roomId, message, isMine);
        m_messagesLayout->addStretch(1);
    } else if (!isMine) {
        setSidebarGlow(m_roomItem[roomId], true);
    }
}

void MainWindow::sendCurrentMessage() {
    // Single-line composer: prevents any confusion about "missing words".
    const QString raw = m_input->text();
    if (raw.trimmed().isEmpty()) return;
    if (!m_client->isConnected()) {
        QMessageBox::information(this, "Not connected", "Connect to the server first.");
        return;
    }

    m_client->sendMessage(m_currentRoomId, raw, QString());
    m_input->clear();
}

void MainWindow::onRoomJoined(const QString& roomId, const QString& userId, const QString& userName) {
    if (roomId == m_globalRoomId) {
        m_onlineUsers[userId] = userName;
        refreshOnlineUsersList();
        return;
    }

    // If YOU join a non-global room (join by ID / created room), open it in the UI.
    if (userId == m_userId) {
        const QString title = roomTitleForId(roomId);
        ensureRoomExists(roomId, title);
        selectRoom(roomId);
        statusBar()->showMessage(QString("Joined %1").arg(title), 2000);
    }
}

void MainWindow::onRoomLeft(const QString& roomId, const QString& userId, const QString& userName) {
    if (roomId != m_globalRoomId) return;
    Q_UNUSED(userName);
    m_onlineUsers.remove(userId);
    refreshOnlineUsersList();
}

void MainWindow::onRoomCreated(const QString& roomId, bool exists) {
    Q_UNUSED(exists);

    // If the user just created a room, attach a friendly label (optional)
    QString label;
    if (!m_pendingRoomTitle.trimmed().isEmpty()) {
        label = QString("👥 %1").arg(m_pendingRoomTitle.trimmed());
    } else if (!m_pendingRoomUsers.isEmpty()) {
        QStringList names;
        for (const auto& uid : m_pendingRoomUsers) {
            names << m_onlineUsers.value(uid, uid.left(8));
        }
        if (!names.isEmpty()) {
            label = QString("👥 Group: %1").arg(names.join(", "));
        }
    }
    if (!label.isEmpty()) {
        m_customRoomTitle[roomId] = label;
    }
    m_pendingRoomTitle.clear();
    m_pendingRoomUsers.clear();

    const QString title = roomTitleForId(roomId);
    ensureRoomExists(roomId, title);
    selectRoom(roomId);
    statusBar()->showMessage(QString("Opened %1").arg(title), 2500);
}


static QStringList selectedOtherUserIds(QListWidget* users, const QString& selfId) {
    QStringList ids;
    for (auto* it : users->selectedItems()) {
        const QString uid = it->data(Qt::UserRole).toString();
        if (uid.isEmpty() || uid == selfId) continue;
        ids << uid;
    }
    ids.removeDuplicates();
    return ids;
}

void MainWindow::startDmWithSelection() {
    if (!m_client->isConnected()) {
        QMessageBox::information(this, "Not connected", "Connect to the server first.");
        return;
    }

    const QStringList userIds = selectedOtherUserIds(m_users, m_userId);
    if (userIds.size() != 1) {
        QMessageBox::information(this, "Private message", "Select exactly 1 user for a DM.");
        return;
    }
    statusBar()->showMessage("Creating private chat…", 2000);
    m_client->createPrivateRoom(userIds.front());
}

void MainWindow::startRoomWithSelection() {
    if (!m_client->isConnected()) {
        QMessageBox::information(this, "Not connected", "Connect to the server first.");
        return;
    }

    const QStringList userIds = selectedOtherUserIds(m_users, m_userId);

    // Optional: give the room a human name (local UI label only)
    m_pendingRoomTitle = m_roomNameInput ? m_roomNameInput->text().trimmed() : QString();
    m_pendingRoomUsers = userIds;
    if (m_roomNameInput) m_roomNameInput->clear();

    if (userIds.size() == 1) {
        QMessageBox::information(this, "Create room", "For 1 person, use DM.\nSelect 2+ users for a group room, or select none to create a room you can share.");
        m_pendingRoomTitle.clear();
        m_pendingRoomUsers.clear();
        return;
    }

    if (userIds.isEmpty()) {
        statusBar()->showMessage("Creating room… (share the room ID with others)", 2500);
    } else {
        statusBar()->showMessage("Creating room…", 2000);
    }
    m_client->createGroupRoom(userIds);
}


void MainWindow::joinRoomFromInput() {
    if (!m_client->isConnected()) {
        QMessageBox::information(this, "Not connected", "Connect to the server first.");
        return;
    }
    const QString roomId = m_joinRoomInput ? m_joinRoomInput->text().trimmed() : QString();
    if (roomId.isEmpty()) return;

    statusBar()->showMessage(QString("Joining %1…").arg(roomId), 2000);
    m_client->joinRoom(roomId);

    if (m_joinRoomInput) m_joinRoomInput->clear();
}

void MainWindow::onUserDoubleClicked(QListWidgetItem* item) {
    if (!item) return;
    m_users->clearSelection();
    item->setSelected(true);
    startDmWithSelection();
}

QString MainWindow::roomTitleForId(const QString& roomId) const {
    if (roomId == m_globalRoomId) return "🌍 Global";

    if (m_customRoomTitle.contains(roomId)) return m_customRoomTitle.value(roomId);

    // private_<idA>_<idB>
    if (roomId.startsWith("private_")) {
        const QStringList parts = roomId.split('_');
        if (parts.size() >= 3) {
            const QString a = parts.at(1);
            const QString b = parts.at(2);
            const QString other = (a == m_userId) ? b : a;
            const QString otherName = m_onlineUsers.value(other, other.left(8));
            return QString("🔒 DM: %1").arg(otherName);
        }
        return "🔒 DM";
    }

    return QString("👥 Room: %1").arg(roomId.left(10));
}

void MainWindow::refreshOnlineUsersList() {
    if (!m_users) return;
    m_users->clear();

    // Sort by name for readability
    QList<QPair<QString, QString>> entries; // (name, id)
    for (auto it = m_onlineUsers.constBegin(); it != m_onlineUsers.constEnd(); ++it) {
        entries.append({it.value(), it.key()});
    }
    std::sort(entries.begin(), entries.end(), [](const auto& l, const auto& r){
        return QString::compare(l.first, r.first, Qt::CaseInsensitive) < 0;
    });

    if (entries.isEmpty()) {
        auto* empty = new QListWidgetItem("(No users online yet)");
        empty->setFlags(empty->flags() & ~Qt::ItemIsSelectable);
        m_users->addItem(empty);
        return;
    }

    for (const auto& e : entries) {
        const QString name = e.first;
        const QString id = e.second;
        const QString prefix = (id == m_userId) ? "🟢 (You)" : "🟢";
        auto* item = new QListWidgetItem(QString("%1 %2  (%3)").arg(prefix, name, id.left(8)));
        item->setData(Qt::UserRole, id);
        m_users->addItem(item);
    }
}

void MainWindow::setSidebarGlow(QListWidgetItem* item, bool glow) {
    if (!item) return;
    if (glow) item->setText("✨ " + item->text());
    else item->setText(item->text().startsWith("✨ ") ? item->text().mid(2) : item->text());
}

void MainWindow::onThemeChanged(const QString& themeName) {
    applyTheme(themeName);
}

void MainWindow::applyTheme(const QString& themeName) {
    const QString t = themeName.trimmed().toLower();
    m_isLightTheme = (t == "light");
    qApp->setStyleSheet(ThemeManager::stylesheet(themeName));
    updateWallpaper();
    // Re-render to refresh bubble contrast rules.
    renderRoom(m_currentRoomId);
}

void MainWindow::updateWallpaper() {
    if (!m_messagesContainer) return;
    if (m_isLightTheme) {
        m_messagesContainer->setStyleSheet(R"QSS(
            QWidget {
                background: qradialgradient(cx:0.25, cy:0.1, radius:1.2,
                    stop:0 rgba(0,180,255,0.12), stop:0.45 rgba(255,0,255,0.06), stop:1 rgba(255,255,255,0.00));
                border-radius: 18px;
            }
        )QSS");
    } else {
        m_messagesContainer->setStyleSheet(R"QSS(
            QWidget {
                background: qradialgradient(cx:0.2, cy:0.1, radius:1.2,
                    stop:0 rgba(0,255,255,0.06), stop:0.45 rgba(255,0,255,0.04), stop:1 rgba(255,255,255,0.00));
                border-radius: 18px;
            }
        )QSS");
    }
}

void MainWindow::toggleEmojiPanel() {
    if (!m_emojiPanel) return;
    m_emojiPanel->setVisible(!m_emojiPanel->isVisible());
    if (m_input) m_input->setFocus();
}


void MainWindow::insertEmoji(const QString& emoji) {
    if (!m_input) return;
    const int pos = m_input->cursorPosition();
    QString t = m_input->text();
    t.insert(pos, emoji);
    m_input->setText(t);
    m_input->setCursorPosition(pos + emoji.size());
    m_input->setFocus();
}
