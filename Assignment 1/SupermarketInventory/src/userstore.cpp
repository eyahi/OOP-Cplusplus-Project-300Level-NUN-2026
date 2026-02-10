#include "userstore.h"

#include "appdata.h"
#include <QCryptographicHash>
#include <QFile>
#include <QFileInfo>
#include <QRandomGenerator>
#include <QSaveFile>
#include <QTextStream>
#include <QStringList>

namespace {

// Hash a password with a provided salt (simple SHA-256 for this app).
QByteArray hashPassword(const QString &password, const QByteArray &salt)
{
    return QCryptographicHash::hash(salt + password.toUtf8(),
                                    QCryptographicHash::Sha256);
}

// Generate a random salt.
QByteArray randomSalt()
{
    QByteArray salt(16, Qt::Uninitialized);
    for (int i = 0; i < salt.size(); ++i) {
        salt[i] = static_cast<char>(QRandomGenerator::global()->generate() & 0xFF);
    }
    return salt;
}

// Parse a CSV line into a UserRecord (hashed CSV format).
bool parseUserRecord(const QString &line, UserStore::UserRecord *out)
{
    const QString trimmed = line.trimmed();
    if (trimmed.isEmpty()) {
        return false;
    }

    const QStringList parts = trimmed.split(",");
    if (parts.size() < 2) {
        return false;
    }

    const QString header = parts[0].trimmed().toLower();
    if (header == "username") {
        return false;
    }

    out->username = parts[0].trimmed();
    if (out->username.isEmpty()) {
        return false;
    }

    if (parts.size() < 3) {
        return false;
    }

    out->salt = QByteArray::fromHex(parts[1].trimmed().toUtf8());
    out->hash = QByteArray::fromHex(parts[2].trimmed().toUtf8());
    out->isAdmin = false;
    if (parts.size() >= 4) {
        const QString role = parts[3].trimmed().toLower();
        out->isAdmin = (role == "admin" || role == "true" || role == "1");
    }
    return !out->salt.isEmpty() && !out->hash.isEmpty();
}

}

namespace UserStore {

QString primaryPath()
{
    return AppData::usersFilePath();
}
//check if file exists
bool loadUsersIfExists(QVector<UserRecord> *records, QString *errorMessage, QString *pathUsed)
{
    if (!records) {
        return false;
    }

    records->clear();
    if (pathUsed) {
        pathUsed->clear();
    }
    
    const QString pathToOpen = primaryPath();
    if (!QFileInfo::exists(pathToOpen)) {
        return true;
    }
    //open file
    if (pathUsed) {
        *pathUsed = pathToOpen;
    }
    //read each line
    QFile file(pathToOpen);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        if (errorMessage) {
            *errorMessage = "Could not open users file.";
        }
        return false;
    }

    QTextStream in(&file);
    while(!in.atEnd()) {
        UserRecord record;
        const QString line = in.readLine();
        if (!parseUserRecord(line, &record)) {
            continue;
        }
        records->push_back(record);
    }

    file.close();
    return true;
}

bool saveUsers(const QVector<UserRecord> &records, QString *errorMessage)
{
    QString dirError;
    if (!AppData::ensureDataDir(&dirError)) {
        if (errorMessage) {
            *errorMessage = dirError;
        }
        return false;
    }

    QSaveFile file(primaryPath());
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        if (errorMessage) {
            *errorMessage = "Could not write users file.";
        }
        return false;
    }

    QTextStream out(&file);
    out << "username,salt,hash,role\n";
    for (const UserRecord &record : records) {
        const QString role = record.isAdmin ? "admin" : "user";
        out << record.username << "," << record.salt.toHex() << "," << record.hash.toHex()
            << "," << role << "\n";
    }

    if (!file.commit()) {
        if (errorMessage) {
            *errorMessage = "Could not finalize users file.";
        }
        return false;
    }
    return true;
}
//loop through and find username and password
bool verifyUser(const QVector<UserRecord> &records,
                const QString &username,
                const QString &password,
                bool *isAdmin)
{
    if (isAdmin) {
        *isAdmin = false;
    }

    for (const UserRecord &record : records) {
        if (record.username != username) {
            continue;
        }
        const QByteArray computed = hashPassword(password, record.salt);
        if (computed == record.hash) {
            if (isAdmin) {
                *isAdmin = record.isAdmin;
            }
            return true;
        }
    }
    return false;
}

bool usernameExists(const QVector<UserRecord> &records, const QString &username)
{
    for (const UserRecord &record : records) {
        if (record.username == username) {
            return true;
        }
    }
    return false;
}

bool anyAdmin(const QVector<UserRecord> &records)
{
    for (const UserRecord &record : records) {
        if (record.isAdmin) {
            return true;
        }
    }
    return false;
}

UserRecord createUser(const QString &username, const QString &password, bool isAdmin)
{
    UserRecord record;
    record.username = username;
    record.salt = randomSalt();
    record.hash = hashPassword(password, record.salt);
    record.isAdmin = isAdmin;
    return record;
}

}
