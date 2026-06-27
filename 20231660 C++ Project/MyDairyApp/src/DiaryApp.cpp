/*
 * Name: Ihenacho Chidubem 20231660, Joachim Ehiagwina 20231811, Sonia Enechi 20231027 
 * File: DiaryApp.cpp
 * Purpose: Implements the logic for authentication and diary management.
 */

#include "DiaryApp.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFile>
#include <QTextStream>
#include <QCryptographicHash>
#include <QDir>
#include <QMessageBox>

// LOGIN DIALOG FUNCTIONS

/*
 * Function: LoginDialog
 * Purpose: Initializes the login screen widgets and layout.
 * Parameters: parent - The parent widget.
 * Memory Note: Qt handles memory management. Widgets (buttons, labels) 
 * are children of 'this' window and are automatically deleted when closed.
 */
LoginDialog::LoginDialog(QWidget *parent) : QDialog(parent) {
    setWindowTitle("Diary Security");
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    messageLabel = new QLabel("Enter Password:");
    passwordField = new QLineEdit();
    passwordField->setEchoMode(QLineEdit::Password); // Mask input for security
    
    // Determine button text based on whether a password exists
    loginButton = new QPushButton(isFirstRun() ? "Create Password" : "Login");

    layout->addWidget(messageLabel);
    layout->addWidget(passwordField);
    layout->addWidget(loginButton);

    // Connect the button click signal to our custom slot
    connect(loginButton, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
}

/*
 * Function: onLoginClicked
 * Purpose: Validates credentials or creates a new password.
 * Returns: void
 */
void LoginDialog::onLoginClicked() {
    QString input = passwordField->text();

    // Input Validation: Ensure password is not empty
    if (input.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Password cannot be empty.");
        return;
    }

    if (isFirstRun()) {
        savePassword(input);
        QMessageBox::information(this, "Success", "Password created successfully!");
        accept(); // Closes dialog with 'Accepted' result
    } else if (checkPassword(input)) {
        accept(); 
    } else {
        QMessageBox::warning(this, "Access Denied", "Incorrect Password!");
        passwordField->clear();
    }
}

/*
 * Function: isFirstRun
 * Purpose: Checks if the password file exists.
 * Returns: true if no password file is found, false otherwise.
 */
bool LoginDialog::isFirstRun() const { 
    return !QFile::exists(passwordFile); 
}

/*
 * Function: savePassword
 * Purpose: Hashes and saves the user's password to disk.
 * Parameters: input - The plain text password to save.
 */
void LoginDialog::savePassword(const QString &input) {
    // Ensure the data directory exists before writing
    if (!QDir("data").exists()) QDir().mkdir("data");
    
    QFile file(passwordFile);
    if (file.open(QIODevice::WriteOnly)) {
        // Use SHA-256 to hash the password before storage
        QByteArray hash = QCryptographicHash::hash(input.toUtf8(), QCryptographicHash::Sha256).toHex();
        file.write(hash);
        file.close(); // Closes the file
    }
}

/*
 * Function: checkPassword
 * Purpose: Compares the input against the stored hash.
 * Parameters: input - The plain text password to check.
 * Returns: true if hashes match, false otherwise.
 */
bool LoginDialog::checkPassword(const QString &input) const {
    QFile file(passwordFile);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray stored = file.readAll();
        // Hash the input and compare it to the stored hash
        QByteArray current = QCryptographicHash::hash(input.toUtf8(), QCryptographicHash::Sha256).toHex();
        return stored == current;
    }
    return false;
}

// MAIN WINDOW FUNCTIONS

/*
 * Function: MainWindow
 * Purpose: Sets up the main UI and loads initial data.
 * Parameters: parent - The parent widget.
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("My Personal Diary");
    resize(800, 600);
    setupUI();
    
    // Ensure data folder exists to prevent file errors later
    if (!QDir("data").exists()) QDir().mkdir("data");
    
    // Load entry for today's date automatically
    loadEntryForDate(QDate::currentDate());
}

/*
 * Function: setupUI
 * Purpose: Configures widgets and layouts for the main window.
 * Returns: void
 */
void MainWindow::setupUI() {
    // Central widget is required for QMainWindow
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    QVBoxLayout *rightLayout = new QVBoxLayout();

    calendar = new QCalendarWidget();
    entryArea = new QTextEdit();
    saveButton = new QPushButton("Save Entry");
    statusLabel = new QLabel("Ready");

    // Styling: Blue background for save button
    saveButton->setStyleSheet("background-color: #2500f7; color: white; padding: 5px;");

    rightLayout->addWidget(entryArea);
    rightLayout->addWidget(saveButton);
    rightLayout->addWidget(statusLabel);

    // Layout ratios: Calendar 1 part, Text Area 2 parts
    mainLayout->addWidget(calendar, 1);
    mainLayout->addLayout(rightLayout, 2);

    // Connect signals to slots in the header file
    connect(calendar, &QCalendarWidget::selectionChanged, this, &MainWindow::onDateChanged);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::onSaveClicked);
}

/*
 * Function: onDateChanged
 * Purpose: Slot called when the user selects a new date.
 */
void MainWindow::onDateChanged() {
    loadEntryForDate(calendar->selectedDate());
}

/*
 * Function: loadEntryForDate
 * Purpose: Loads text from a file into the text area.
 * Parameters: date - The date to load.
 */
void MainWindow::loadEntryForDate(const QDate &date) {
    QFile file(getFilePath(date));
    
    // Error Handling: Check if file opens successfully
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        entryArea->setText(in.readAll());
        statusLabel->setText("Loaded entry for: " + date.toString());
        file.close();
    } else {
        entryArea->clear(); // Clear text if no entry exists
        statusLabel->setText("No entry found for: " + date.toString());
    }
}

/*
 * Function: onSaveClicked
 * Purpose: Saves the current text to a file.
 */
void MainWindow::onSaveClicked() {
    QDate selectedDate = calendar->selectedDate();
    QFile file(getFilePath(selectedDate));
    
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << entryArea->toPlainText();
        statusLabel->setText("Saved successfully!");
        file.close();
    } else {
        QMessageBox::critical(this, "File Error", "Could not save file!"); // Error handling
    }
}

/*
 * Function: getFilePath
 * Purpose: Constructs the file path for a given date.
 * Parameters: date - The date object.
 * Returns: A string representing the file path
 */
QString MainWindow::getFilePath(const QDate &date) const {
    return QString("data/%1.txt").arg(date.toString("yyyy-MM-dd"));
}