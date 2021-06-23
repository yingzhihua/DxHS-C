#include "systemname.h"
#include "ui_systemname.h"
#include "../module/exglobal.h"
#include "../module/uihandler.h"

SystemName::SystemName(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SystemName)
{
    ui->setupUi(this);
    ui->pushButton->text()="确定";

}

SystemName::~SystemName()
{
    delete ui;
}

void SystemName::on_pushButton_clicked()
{
    ExGlobal::getPtr()->setHospitalName(ui->lineEdit_hospital->text());
    ExGlobal::getPtr()->setSysName(ui->lineEdit_device->text());
    ExGlobal::getPtr()->setPrintType(ui->comboBox_print->currentIndex());
    //ui->lineEdit_hospital->setText(ExGlobal::getPtr()->hospitalName());
}

void SystemName::on_pushButton_2_clicked()
{
     //delete ui;
     UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}
