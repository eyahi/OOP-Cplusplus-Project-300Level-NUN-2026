#ifndef APPDATA_H
#define APPDATA_H

#include <QString>

namespace AppData {
// AppData root directory for this app.
QString dataDir();
// Primary inventory storage path.
QString inventoryFilePath();
// Primary users storage path.
QString usersFilePath();
// Ensure the AppData directory exists on disk.
bool ensureDataDir(QString *errorMessage = nullptr);
}

#endif
