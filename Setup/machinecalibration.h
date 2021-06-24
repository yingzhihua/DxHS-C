#ifndef MACHINECALIBRATION_H
#define MACHINECALIBRATION_H

#include <QDialog>

namespace Ui {
class MachineCalibration;
}

class MachineCalibration : public QDialog
{
    Q_OBJECT

public:
    explicit MachineCalibration(QWidget *parent = nullptr);
    ~MachineCalibration();
    void load_data(void);
    int predoorpos=0;

private slots:
    void on_pButton_machine_back_clicked();

    void on_pButton_v1_hard_clicked();

    void on_pButton_v1_soft_clicked();

    void on_pButton_v1_work_clicked();

    void on_pButton_v1_set_clicked();

    void on_pButton_v1_save_clicked();

    void on_pButton_v2_hard_clicked();

    void on_pButton_v2_soft_clicked();

    void on_pButton_v2_work_clicked();

    void on_pButton_v2_set_clicked();

    void on_pButton_v2_save_clicked();

    void on_pButton_v3_hard_clicked();

    void on_pButton_v3_soft_clicked();

    void on_pButton_v3_work_clicked();

    void on_pButton_v3_set_clicked();

    void on_pButton_v3_save_clicked();

    void on_pButton_v4_hard_clicked();

    void on_pButton_v4_soft_clicked();

    void on_pButton_v4_work_clicked();

    void on_pButton_v4_set_clicked();

    void on_pButton_v4_save_clicked();

    void on_pButton_door_hard_clicked();

    void on_pButton_door_soft_clicked();

    void on_pButton_door_work_clicked();

    void on_pButton_door_set_clicked();

    void on_pButton_door_save_clicked();

    void on_pButton_push_hard_clicked();

    void on_pButton_push_soft_clicked();

    void on_pButton_push_work_clicked();

    void on_pButton_push_set_clicked();

    void on_pButton_push_save_clicked();

    void on_pButton_door_in_clicked();

    void on_pButton_door_out_clicked();

private:
    Ui::MachineCalibration *ui;
};

#endif // MACHINECALIBRATION_H
