#ifndef TESTSETUP_H
#define TESTSETUP_H

#include <QDialog>

namespace Ui {
class TestSetup;
}

class TestSetup : public QDialog
{
    Q_OBJECT

public:
    explicit TestSetup(QWidget *parent = nullptr);
    ~TestSetup();

private:
    Ui::TestSetup *ui;
};

#endif // TESTSETUP_H
