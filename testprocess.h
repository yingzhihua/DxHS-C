#ifndef TESTPROCESS_H
#define TESTPROCESS_H

#include <QDialog>

namespace Ui {
class TestProcess;
}

class TestProcess : public QDialog
{
    Q_OBJECT

public:
    explicit TestProcess(QWidget *parent = nullptr);
    ~TestProcess();

private:
    Ui::TestProcess *ui;
};

#endif // TESTPROCESS_H
