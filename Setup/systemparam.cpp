#include "systemparam.h"
#include "ui_systemparam.h"

#include "../module/uihandler.h"
#include "../module/sequence.h"
#include "../module/exglobal.h"



SystemParam::SystemParam(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SystemParam)
{
    ui->setupUi(this);
}

SystemParam::~SystemParam()
{
    delete ui;
}

void SystemParam::showEvent(QShowEvent *event){
    Q_UNUSED(event);

    if (ExGlobal::getPtr()->getCaliParam("ProjectMode")==0)
        ui->rButton_project->setChecked(true);
    else if (ExGlobal::getPtr()->getCaliParam("ProjectMode")==2)
        ui->rButton_check->setChecked(true);
    else if (ExGlobal::getPtr()->getCaliParam("ProjectMode")==3)
        ui->rButton_play->setChecked(true);
    else if (ExGlobal::getPtr()->getCaliParam("ProjectMode")==4)
        ui->rButton_test->setChecked(true);
    else
        ui->rButton_normal->setChecked(true);

}

void SystemParam::hideEvent(QHideEvent *event){
    Q_UNUSED(event);

}

void SystemParam::on_rButton_normal_clicked()
{
     ExGlobal::getPtr()->updateCaliParam("ProjectMode",1);
}

void SystemParam::on_rButton_project_clicked()
{
     ExGlobal::getPtr()->updateCaliParam("ProjectMode",0);
}

void SystemParam::on_rButton_check_clicked()
{
     ExGlobal::getPtr()->updateCaliParam("ProjectMode",2);
}

void SystemParam::on_rButton_play_clicked()
{
     ExGlobal::getPtr()->updateCaliParam("ProjectMode",3);
}

void SystemParam::on_rButton_test_clicked()
{
     ExGlobal::getPtr()->updateCaliParam("ProjectMode",4);
}

void SystemParam::on_pButton_machine_back_clicked()
{
     UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}
