#ifndef PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_H

#include <QString>
#include <QWidget>

// handles password stuff for the diary
class PasswordManager {
public:
    PasswordManager(const QString& passwordFilePath);

    // load and save password hash
    void loadPassword();
    void savePassword(const QString& passwordHash);
    void removePassword();

    // check if password is set
    bool hasPassword() const;

    // password verification and changing
    bool verifyPassword(QWidget* parent);
    bool changePassword(QWidget* parent);
    bool forgotPasswordRecovery(QWidget* parent);

    // hash a password using SHA256
    QString hashPassword(const QString& password) const;

private:
    // Windows-specific password recovery
    bool authenticateWithWindowsPassword(const QString& password);

    QString m_passwordFile;
    QString m_currentPasswordHash;
};

#endif // PASSWORD_MANAGER_H
