#ifndef USERSTORE_H
#define USERSTORE_H

#include <QByteArray>
#include <QString>
#include <QVector>

// Centralized user file helpers shared by login and signup.
namespace UserStore {

// Stored user record (hashed format).
struct UserRecord {
    QString username;
    QByteArray salt;
    QByteArray hash;
    bool isAdmin = false;
};

// File locations.
QString primaryPath();

// Load users if a file exists. If none exists, records is cleared and pathUsed is empty.
bool loadUsersIfExists(QVector<UserRecord> *records, QString *errorMessage, QString *pathUsed);

// Save all users to the primary file (legacy records are migrated to hashed format).
bool saveUsers(const QVector<UserRecord> &records, QString *errorMessage);

// Verify credentials against loaded records.
bool verifyUser(const QVector<UserRecord> &records,
                const QString &username,
                const QString &password,
                bool *isAdmin);

// Simple queries on loaded records.
bool usernameExists(const QVector<UserRecord> &records, const QString &username);
bool anyAdmin(const QVector<UserRecord> &records);

// Create a new hashed user record.
UserRecord createUser(const QString &username, const QString &password, bool isAdmin);

}

#endif
