#include <QApplication>
#include "DiaryApp.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyle("Fusion"); // Makes it look modern

    LoginDialog login;
    // Only show the Diary (MainWindow) if the Login (LoginDialog) is "Accepted"
    if (login.exec() == QDialog::Accepted) {
        MainWindow window;
        window.show();
        return app.exec(); // Keep the app running
    }
    return 0; // Exit if login fails or window is closed
}