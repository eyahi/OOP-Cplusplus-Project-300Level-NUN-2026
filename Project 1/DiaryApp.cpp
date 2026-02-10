 #include "DiaryApp.h"
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QDateEdit>
#include <QMessageBox>
#include <QDir>
#include <QSplitter>
#include <QStackedWidget>
#include <QLineEdit>
#include <chrono>
#include <ctime>
#include <algorithm>

// button styling - kept these separate so we can change them easily
const QString DiaryApp::BUTTON_STYLE =
    "QPushButton { "
    "background-color: #8A897C; "
    "color: white; "
    "padding: 15px; "
    "font-size: 16px; "
    "font-weight: bold; "
    "border: none; "
    "border-radius: 10px; "
    "margin: 10px; "
    "} "
    "QPushButton:hover { "
    "background-color: #FBA823; "
    "}";

const QString DiaryApp::RED_BUTTON_STYLE =
    "QPushButton { "
    "background-color: #e74c3c; "
    "color: white; "
    "padding: 15px; "
    "font-size: 16px; "
    "font-weight: bold; "
    "border: none; "
    "border-radius: 10px; "
    "margin: 10px; "
    "} "
    "QPushButton:hover { "
    "background-color: #c0392b; "
    "}";

DiaryApp::DiaryApp(QWidget* parent) : QMainWindow(parent) {
    setWindowTitle("My Diary");
    resize(900, 600);

    // set up file paths for storing entries and password
    m_dataFile = QDir::homePath() + "/diary_entries.txt";
    m_passwordFile = QDir::homePath() + "/diary_password.txt";

    // initialize password manager and load saved password if exists
    m_passwordManager = new PasswordManager(m_passwordFile);
    m_passwordManager->loadPassword();

    m_diaryStorage = new DiaryStorage(m_dataFile);

    // build the interface and load existing entries
    setupUI();
    m_diaryStorage->loadEntries(m_entries);
    updateEntryList();
}

DiaryApp::~DiaryApp() {
    delete m_passwordManager;
    delete m_diaryStorage;
}

void DiaryApp::setupUI() {
    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setStyleSheet("background-color: #141414;");

    QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

    // left panel setup - this shows the list of past entries
    QWidget* leftPanel = new QWidget();
    leftPanel->setStyleSheet(
        "background-color: #BDBBB0; "
        "color: black; "
        "border: 1px solid #BDBBB0; "
        "border-radius:20px;");
    QVBoxLayout* leftLayout = new QVBoxLayout(leftPanel);

    QLabel* entriesLabel = new QLabel("Past Entries:");
    entriesLabel->setStyleSheet("font-weight: bold; font-size: 14px;");

    m_entryList = new QListWidget();
    m_entryList->setStyleSheet("QListWidget { border: 1px solid white; }");

    leftLayout->addWidget(entriesLabel);
    leftLayout->addWidget(m_entryList);

    // right side uses stacked widget to switch between menu and editor
    m_stackedWidget = new QStackedWidget();
    createMainMenu();
    createEntryEditor();

    m_stackedWidget->addWidget(m_mainMenuWidget);
    m_stackedWidget->addWidget(m_entryEditorWidget);
    m_stackedWidget->setCurrentWidget(m_mainMenuWidget);

    // splitter lets user resize panels
    QSplitter* splitter = new QSplitter(Qt::Horizontal);
    splitter->addWidget(leftPanel);
    splitter->addWidget(m_stackedWidget);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 2);

    mainLayout->addWidget(splitter);

    // double click on entry to open it
    connect(m_entryList, &QListWidget::itemDoubleClicked,
            this, &DiaryApp::onEntryDoubleClicked);
}

void DiaryApp::createMainMenu() {
    m_mainMenuWidget = new QWidget();
    QVBoxLayout* menuLayout = new QVBoxLayout(m_mainMenuWidget);

    QLabel* titleLabel = new QLabel("Main Menu");
    titleLabel->setStyleSheet(
        "font-size: 40px; "
        "font-weight: bold; "
        "color: white; "
        "padding: 25px");
    titleLabel->setAlignment(Qt::AlignCenter);

    menuLayout->addWidget(titleLabel);
    menuLayout->addSpacing(30);

    // create all the menu buttons
    QPushButton* newEntryBtn = new QPushButton("Add New Entry");
    QPushButton* setPasswordBtn = new QPushButton("Set/Change Password");
    QPushButton* removePasswordBtn = new QPushButton("Remove Password");
    QPushButton* exitBtn = new QPushButton("Exit");

    newEntryBtn->setStyleSheet(BUTTON_STYLE);
    setPasswordBtn->setStyleSheet(BUTTON_STYLE);
    removePasswordBtn->setStyleSheet(BUTTON_STYLE);
    exitBtn->setStyleSheet(RED_BUTTON_STYLE); // red for exit

    menuLayout->addWidget(newEntryBtn);
    menuLayout->addWidget(setPasswordBtn);
    menuLayout->addWidget(removePasswordBtn);
    menuLayout->addWidget(exitBtn);
    menuLayout->addStretch();

    // hook up button clicks
    connect(newEntryBtn, &QPushButton::clicked,
            this, &DiaryApp::showEntryEditor);
    connect(setPasswordBtn, &QPushButton::clicked,
            this, &DiaryApp::changePassword);
    connect(removePasswordBtn, &QPushButton::clicked,
            this, &DiaryApp::removePassword);
    connect(exitBtn, &QPushButton::clicked,
            this, &DiaryApp::exitApp);
}

void DiaryApp::createEntryEditor() {
    m_entryEditorWidget = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(m_entryEditorWidget);

    // back button to return to menu
    QPushButton* backBtn = new QPushButton("Back to Menu");
    backBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #999999; "
        "color: black; "
        "padding: 4px; "
        "font-weight: bold; "
        "border-radius: 3px; "
        "}"
        "QPushButton:hover { "
        "background-color: #94A0B3; "
        "}");
    connect(backBtn, &QPushButton::clicked, this, &DiaryApp::showMainMenu);

    // title input
    QLabel* titleLabel = new QLabel("Title:");
    titleLabel->setStyleSheet("font-weight: bold; font-size: 14px;");

    m_titleEdit = new QLineEdit();
    m_titleEdit->setPlaceholderText("Enter title...");
    m_titleEdit->setStyleSheet(
        "QLineEdit { "
        "border: 0.5px solid #252525; "
        "padding: 8px; "
        "font-size: 13px; "
        "}");

    // date and time display
    QHBoxLayout* dateTimeLayout = new QHBoxLayout();
    QLabel* dateLabel = new QLabel("Date:");
    dateLabel->setStyleSheet("color: #999999; font-weight: bold;");

    m_dateEdit = new QDateEdit();
    m_dateEdit->setDate(QDate::currentDate());
    m_dateEdit->setCalendarPopup(true);

    m_timeLabel = new QLabel("Time: --:--:--");
    m_timeLabel->setStyleSheet("font-weight: bold; color: #999999;");

    dateTimeLayout->addWidget(dateLabel);
    dateTimeLayout->addWidget(m_dateEdit);
    dateTimeLayout->addSpacing(20);
    dateTimeLayout->addWidget(m_timeLabel);
    dateTimeLayout->addStretch();

    // main text area for entry content
    QLabel* contentLabel = new QLabel("Entry:");
    contentLabel->setStyleSheet("font-weight: bold; font-size: 14px;");

    m_entryText = new QTextEdit();
    m_entryText->setPlaceholderText("...");
    m_entryText->setStyleSheet("QTextEdit { font-size: 12px;}");

    // save and delete buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_saveBtn = new QPushButton("Save Entry");
    m_deleteBtn = new QPushButton("Delete Entry");
    m_saveBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #E9EDC9; "
        "color: black; "
        "padding: 8px; "
        "font-weight: bold; "
        "}");
    m_deleteBtn->setStyleSheet(
        "QPushButton { "
        "background-color: #FE7171; "
        "color: black; "
        "padding: 8px; "
        "font-weight: bold; "
        "}");

    buttonLayout->addWidget(m_saveBtn);
    buttonLayout->addWidget(m_deleteBtn);

    // add everything to layout
    rightLayout->addWidget(backBtn);
    rightLayout->addWidget(titleLabel);
    rightLayout->addWidget(m_titleEdit);
    rightLayout->addLayout(dateTimeLayout);
    rightLayout->addWidget(contentLabel);
    rightLayout->addWidget(m_entryText);
    rightLayout->addLayout(buttonLayout);

    connect(m_saveBtn, &QPushButton::clicked, this, &DiaryApp::saveEntry);
    connect(m_deleteBtn, &QPushButton::clicked, this, &DiaryApp::deleteEntry);
}

// get current time as string in HH:MM:SS format
QString DiaryApp::getCurrentTimeString() const {
    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    std::tm* localTime = std::localtime(&nowTime);

    char buffer[9];
    std::strftime(buffer, sizeof(buffer), "%H:%M:%S", localTime);
    return QString(buffer);
}

void DiaryApp::showMainMenu() {
    m_stackedWidget->setCurrentWidget(m_mainMenuWidget);
}

void DiaryApp::showEntryEditor() {
    // check password before letting user create new entry
    if (!m_passwordManager->verifyPassword(this)) {
        return;
    }

    // clear fields and set to current date/time
    m_titleEdit->clear();
    m_entryText->clear();
    m_dateEdit->setDate(QDate::currentDate());
    m_currentEntryTime = getCurrentTimeString();
    m_timeLabel->setText("Time: " + m_currentEntryTime);
    m_stackedWidget->setCurrentWidget(m_entryEditorWidget);
}

void DiaryApp::changePassword() {
    m_passwordManager->changePassword(this);
}

void DiaryApp::removePassword() {
    if (!m_passwordManager->hasPassword()) {
        QMessageBox::information(this, "No Password",
                                 "There is no password set for this diary.");
        return;
    }

    // verify before removing
    if (!m_passwordManager->verifyPassword(this)) {
        return;
    }

    // double check they want to do this
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this, "Remove Password",
        "Are you sure you want to remove the password protection?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_passwordManager->removePassword();
        QMessageBox::information(this, "Success",
                                 "Password removed! Your diary is no longer protected.");
    }
}

void DiaryApp::exitApp() {
    // confirm before closing
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this, "Exit",
        "Are you sure you want to exit?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QApplication::quit();
    }
}

void DiaryApp::onEntryDoubleClicked() {
    // check password before opening entry
    if (!m_passwordManager->verifyPassword(this)) {
        return;
    }

    // load the selected entry into the editor
    int currentRow = m_entryList->currentRow();
    if (currentRow >= 0 && currentRow < m_entries.size()) {
        const DiaryEntry& entry = m_entries[currentRow];
        m_titleEdit->setText(entry.getTitle());
        m_dateEdit->setDate(QDate::fromString(entry.getDate(), "yyyy-MM-dd"));
        m_currentEntryTime = entry.getTime();
        m_timeLabel->setText("Time: " + m_currentEntryTime);
        m_entryText->setPlainText(entry.getContent());
        m_stackedWidget->setCurrentWidget(m_entryEditorWidget);
    }
}

// check if entry with same title and date already exists
int DiaryApp::findExistingEntry(const QString& title, const QString& date) const {
    for (int i = 0; i < m_entries.size(); ++i) {
        if (m_entries[i].getTitle() == title && m_entries[i].getDate() == date) {
            return i;
        }
    }
    return -1; // not found
}

void DiaryApp::saveEntry() {
    QString title = m_titleEdit->text().trimmed();
    QString date = m_dateEdit->date().toString("yyyy-MM-dd");
    QString content = m_entryText->toPlainText().trimmed();

    // make sure they actually entered something
    if (title.isEmpty()) {
        QMessageBox::warning(this, "Empty Title",
                             "Please enter a title for your entry.");
        return;
    }

    if (content.isEmpty()) {
        QMessageBox::warning(this, "Empty Entry",
                             "Please write something before saving.");
        return;
    }

    // update existing entry or create new one
    int existingIndex = findExistingEntry(title, date);

    if (existingIndex >= 0) {
        // updating existing entry
        m_entries[existingIndex].setContent(content);
        m_entries[existingIndex].setTime(m_currentEntryTime);
    } else {
        // new entry
        DiaryEntry newEntry(title, date, m_currentEntryTime, content);
        m_entries.append(newEntry);
    }

    // save to file
    if (m_diaryStorage->saveEntries(m_entries)) {
        updateEntryList();
        QMessageBox::information(this, "Saved", "Entry saved successfully!");
        showMainMenu();
    } else {
        QMessageBox::critical(this, "Error", "Could not save entry!");
    }
}

void DiaryApp::deleteEntry() {
    int currentRow = m_entryList->currentRow();
    if (currentRow < 0) {
        QMessageBox::warning(this, "No Selection",
                             "Please select an entry to delete.");
        return;
    }

    // confirm deletion
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this, "Confirm Delete",
        "Are you sure you want to delete this entry?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_entries.removeAt(currentRow);
        m_diaryStorage->saveEntries(m_entries);
        updateEntryList();
        m_titleEdit->clear();
        m_entryText->clear();
        QMessageBox::information(this, "Deleted", "Entry deleted successfully!");
        showMainMenu();
    }
}

// sort entries by date and time, newest first
void DiaryApp::sortEntries() {
    std::sort(m_entries.begin(), m_entries.end(),
              [](const DiaryEntry& a, const DiaryEntry& b) {
                  if (a.getDate() != b.getDate()) {
                      return a.getDate() > b.getDate();
                  }
                  return a.getTime() > b.getTime();
              });
}

void DiaryApp::updateEntryList() {
    m_entryList->clear();
    sortEntries();

    // add each entry to the list with title, date, and time
    for (const auto& entry : m_entries) {
        QString displayText = QString(" %1\n %2   %3")
        .arg(entry.getTitle())
            .arg(entry.getDate())
            .arg(entry.getTime());
        m_entryList->addItem(displayText);
    }
}
