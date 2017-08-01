#include "simpletimer.h"
#include "ui_simpletimer.h"

Simpletimer::Simpletimer(QWidget *parent) : QMainWindow(parent) {
    ui = new Ui::Simpletimer;
    ui->setupUi(this);
}

Simpletimer::~Simpletimer() {
    delete ui;
}
