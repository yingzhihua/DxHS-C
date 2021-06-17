#ifndef TESTLOOP_H
#define TESTLOOP_H

#include <QDialog>

namespace Ui {
class TestLoop;
}

class TestLoop : public QDialog
{
    Q_OBJECT

public:
    explicit TestLoop(QWidget *parent = nullptr);
    ~TestLoop();

private:
    Ui::TestLoop *ui;
};

#endif // TESTLOOP_H
