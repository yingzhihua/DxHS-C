#include "projectparam.h"
#include "ui_projectparam.h"

#include "../module/uihandler.h"
#include "../module/sequence.h"
#include "../module/exglobal.h"

ProjectParam::ProjectParam(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectParam)
{
    ui->setupUi(this);
}

ProjectParam::~ProjectParam()
{
    delete ui;
}

void ProjectParam::load_data(void)
{
  ui->spinBox_high->setValue( ExGlobal::getPtr()->getCaliParam("LiquidsHeight"));
  ui->spinBox_pre->setValue(ExGlobal::getPtr()->getCaliParam("DryWet"));
}

void ProjectParam::on_pButton_machine_back_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}


void ProjectParam::on_rButton_c1_clicked()
{
    ExGlobal::getPtr()->updateCaliParam("ClearMode",0);
}

void ProjectParam::on_rButton_c2_clicked()
{
    ExGlobal::getPtr()->updateCaliParam("ClearMode",1);
}

void ProjectParam::on_rButton_c3_clicked()
{
    ExGlobal::getPtr()->updateCaliParam("ClearMode",2);
}

void ProjectParam::on_rButton_c4_clicked()
{
    ExGlobal::getPtr()->updateCaliParam("ClearMode",3);
}

void ProjectParam::on_rButton_c5_clicked()
{
    ExGlobal::getPtr()->updateCaliParam("ClearMode",4);
}

void ProjectParam::on_rButton_b1_clicked()
{
     ExGlobal::getPtr()->updateCaliParam("MachineMode",0);
}

void ProjectParam::on_rButton_b2_clicked()
{
    ExGlobal::getPtr()->updateCaliParam("MachineMode",1);
}

void ProjectParam::on_spinBox_high_valueChanged(int arg1)
{
    if (arg1 != ExGlobal::getPtr()->getCaliParam("LiquidsHeight"))
        ExGlobal::getPtr()->updateCaliParam("LiquidsHeight",arg1);
}

void ProjectParam::on_spinBox_pre_valueChanged(const QString &arg1)
{

}

void ProjectParam::on_spinBox_pre_valueChanged(int arg1)
{
    if (arg1 != ExGlobal::getPtr()->getCaliParam("DryWet"))
        ExGlobal::getPtr()->updateCaliParam("DryWet",arg1);
}
