#ifndef STARTUP_H
#define STARTUP_H

#include <QDialog>

#include "module/sequence.h"

namespace Ui {
class Startup;
}

class Startup : public QDialog
{
    Q_OBJECT

public:
    explicit Startup(QWidget *parent = nullptr);
    ~Startup();

private:
    Ui::Startup *ui;

public slots:
    void sequenceFinish(Sequence::SequenceResult result);
private slots:
    void on_Home_Startup_btOpenDoor_clicked();
};

#endif // STARTUP_H
