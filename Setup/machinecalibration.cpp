#include "machinecalibration.h"
#include "ui_machinecalibration.h"

MachineCalibration::MachineCalibration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MachineCalibration)
{
    ui->setupUi(this);
}

MachineCalibration::~MachineCalibration()
{
    delete ui;
}
