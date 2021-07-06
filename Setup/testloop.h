#ifndef TESTLOOP_H
#define TESTLOOP_H

#include <QDialog>
#include "../module/sequence.h"

namespace Ui {
class TestLoop;
}

class TestLoop : public QDialog
{
    Q_OBJECT

public:
    explicit TestLoop(QWidget *parent = nullptr);
    ~TestLoop();
    void load_data(void);
    void sequenceFinish(Sequence::SequenceResult result);

private slots:
    void on_pButton_machine_back_clicked();

    void on_pButton_enter_test_clicked();

    void on_pButton_exit_test_clicked();

private:
    Ui::TestLoop *ui;
};

#endif // TESTLOOP_H
