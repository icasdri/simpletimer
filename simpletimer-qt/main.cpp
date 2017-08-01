#include "simpletimer.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Simpletimer window;
    window.show();

    return app.exec();
}
