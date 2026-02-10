#include "appdata.h"

#include <QDir>
#include <QStandardPaths>

namespace AppData {

QString dataDir()
{
    // Prefer the OS-specific AppData location.
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if (path.isEmpty()) {
        // Fallback for unusual environments.
        path = QDir::currentPath() + QDir::separator() + "data";
    }
    return QDir::fromNativeSeparators(path);
}

bool ensureDataDir(QString *errorMessage)
{
    // Ensure the data directory exists before writing files.
    QDir dir(dataDir());
    if (dir.exists()) {
        return true;
    }
    if (dir.mkpath(".")) {
        return true;
    }
    if (errorMessage) {
        *errorMessage = "Could not create data directory: " + dir.absolutePath();
    }
    return false;
}

QString inventoryFilePath()
{
    // Main inventory storage path.
    return dataDir() + QDir::separator() + "inventory.csv";
}

QString usersFilePath()
{
    // Main users storage path.
    return dataDir() + QDir::separator() + "users.csv";
}

}
