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
    void load_data(void);

private slots:
    void on_pButton_machine_back_clicked();

    void on_pButton_testset_set_clicked();

    void on_pButton_testset_load_clicked();

    void on_pButton_testset_test_clicked();

    void on_checkBox_ChildImage_clicked();

    void on_checkBox_AutoFocus_clicked();

    void on_checkBox_qrCode_clicked();

    void on_spinBox_pox_valueChanged(int arg1);

    void on_pButton_testset_2_Q_clicked();

private:
    Ui::TestSetup *ui;
};

#endif // TESTSETUP_H
