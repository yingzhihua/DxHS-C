#include "systemname.h"
#include "ui_systemname.h"
#include "../module/exglobal.h"


SystemName::SystemName(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SystemName)
{
    ui->setupUi(this);
}

SystemName::~SystemName()
{
    delete ui;
}


void SystemName::on_commandLinkButton_clicked()
{
   // ExGlobal::HospitalName =lineEdit_hospital     ;//lineEdit_hospital.text;
   // ExGlobal::SysName = lineEdit_device.text;
    //ExGlobal::PrintType = comboBox_print.currentIndex;
}
