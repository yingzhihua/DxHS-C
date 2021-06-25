#ifndef TESTPROCESS_H
#define TESTPROCESS_H

#include <QDialog>

#include "module/sequence.h"
#include "module/exglobal.h"
#include "module/uihandler.h"

namespace Ui {
class TestProcess;
}

class TestProcess : public QDialog
{
    Q_OBJECT

public:
    explicit TestProcess(QWidget *parent = nullptr);
    ~TestProcess();

protected:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private slots:
    void on_Home_TestProcess_btCancelTest_clicked();

    void on_SequenceFinish(Sequence::SequenceResult result);

    void on_ProcessFinish(int total, int finish);

private:
    Ui::TestProcess *ui;
};

#endif // TESTPROCESS_H
