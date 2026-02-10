#ifndef DIARY_APP_H
#define DIARY_APP_H

#include "DiaryEntry.h"
#include "PasswordManager.h"
#include "DiaryStorage.h"
#include <QMainWindow>
#include <QList>

class QLineEdit;
class QTextEdit;
class QListWidget;
class QDateEdit;
class QLabel;
class QPushButton;
class QStackedWidget;
class QWidget;

// main window for the diary app
class DiaryApp : public QMainWindow {
    Q_OBJECT

public:
    DiaryApp(QWidget* parent = nullptr);
    ~DiaryApp();

private slots:
    // navigation
    void showMainMenu();
    void showEntryEditor();

    // entry operations
    void saveEntry();
    void deleteEntry();
    void onEntryDoubleClicked();

    // password stuff
    void changePassword();
    void removePassword();

    void exitApp();

private:
    // setup functions
    void setupUI();
    void createMainMenu();
    void createEntryEditor();
    void updateEntryList();
    void sortEntries();

    // helper functions
    QString getCurrentTimeString() const;
    int findExistingEntry(const QString& title, const QString& date) const;

    // UI components
    QLineEdit* m_titleEdit;
    QTextEdit* m_entryText;
    QListWidget* m_entryList;
    QDateEdit* m_dateEdit;
    QLabel* m_timeLabel;
    QPushButton* m_saveBtn;
    QPushButton* m_deleteBtn;
    QStackedWidget* m_stackedWidget;
    QWidget* m_mainMenuWidget;
    QWidget* m_entryEditorWidget;

    // data
    QList<DiaryEntry> m_entries;
    QString m_currentEntryTime;

    // managers for password and storage
    PasswordManager* m_passwordManager;
    DiaryStorage* m_diaryStorage;

    // file paths
    QString m_dataFile;
    QString m_passwordFile;

    // button styles
    static const QString BUTTON_STYLE;
    static const QString RED_BUTTON_STYLE;
};

#endif // DIARY_APP_H
