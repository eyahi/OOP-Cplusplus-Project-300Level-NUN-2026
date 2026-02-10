#include "DiaryApp.h"
#include <QApplication>

// entry point for the diary app
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // create and show the main window
    DiaryApp diary;
    diary.show();

    return app.exec();
}
