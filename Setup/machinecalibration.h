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

private:
    Ui::MachineCalibration *ui;
};

#endif // MACHINECALIBRATION_H
