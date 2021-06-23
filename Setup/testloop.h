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
    void load_data(void);

private slots:
    void on_pButton_machine_back_clicked();

    void on_pButton_enter_test_clicked();

private:
    Ui::TestLoop *ui;
};

#endif // TESTLOOP_H
