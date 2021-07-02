#ifndef SYSTEMPARAM_H
#define SYSTEMPARAM_H

#include <QDialog>

namespace Ui {
class SystemParam;
}

class SystemParam : public QDialog
{
    Q_OBJECT

protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

public:
    explicit SystemParam(QWidget *parent = nullptr);
    ~SystemParam();
   // virtual void timerEvent(QTimerEvent *e);

private slots:
    void on_rButton_normal_clicked();

    void on_rButton_project_clicked();

    void on_rButton_check_clicked();

    void on_rButton_play_clicked();

    void on_rButton_test_clicked();

    void on_pButton_machine_back_clicked();

private:
    Ui::SystemParam *ui;
};

#endif // SYSTEMPARAM_H
