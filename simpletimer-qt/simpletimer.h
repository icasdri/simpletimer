#include <QMainWindow>

namespace Ui {
    class Simpletimer;
}

class Simpletimer : public QMainWindow {
    Q_OBJECT

    public:
        explicit Simpletimer(QWidget *parent = 0);
        ~Simpletimer();

    private:
        Ui::Simpletimer *ui;
};
