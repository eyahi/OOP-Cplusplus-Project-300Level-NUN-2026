/*
 * Name: Ihenacho Chidubem 20231660, Joachim Ehiagwina 20231811, Sonia Enechi 20231027 
 * File: DiaryApp.h
 * Purpose: Defines the classes for the Login Dialog and Main Window.
 */

#ifndef DIARYAPP_H
#define DIARYAPP_H

#include <QMainWindow>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QCalendarWidget>
#include <QTextEdit>

//       THE LOGIN WINDOW CLASS 
/*
 * Class: LoginDialog
 * Purpose: Handles user authentication before the main app opens.
 * Notes: Uses SHA-256 hashing for secure password storage.
 */
class LoginDialog : public QDialog {
    Q_OBJECT
public:
    //  Sets up the login window layout
    explicit LoginDialog(QWidget *parent = nullptr);

private slots:
    //  Called when the login button is clicked
    void onLoginClicked();

private:
    QLineEdit *passwordField;
    QPushButton *loginButton;
    QLabel *messageLabel;
    const QString passwordFile = "data/password.dat";

    // Checks if the password file exists
    bool isFirstRun() const;

    //  Saves the hashed password to a file
    void savePassword(const QString &input);

    //  Verifies the input against the stored hash
    bool checkPassword(const QString &input) const;
};

//     THE MAIN DIARY WINDOW CLASS 
/*
 * Class: MainWindow
 * Purpose: The primary interface for the diary application.
 * Notes: Handles file I/O for saving and loading entries.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    //  Initializes the main window and loads today's entry
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    //  Triggered when user selects a new date on the calendar
    void onDateChanged();

    //  Triggered when user clicks the "Save" button
    void onSaveClicked();

private:
    QCalendarWidget *calendar;
    QTextEdit *entryArea;
    QPushButton *saveButton;
    QLabel *statusLabel;

    //  Configures the visual layout of widgets
    void setupUI();

    //  Reads the text file for a specific date
    void loadEntryForDate(const QDate &date);

    //  Generates the filename string (e.g., "data/2026-01-21.txt")
    QString getFilePath(const QDate &date) const;
};

#endif