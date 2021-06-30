#include "version2.h"
#include "ui_version2.h"


#include "../module/uihandler.h"
#include "../module/sequence.h"
#include "../module/exglobal.h"


Version2::Version2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Version2)
{
    ui->setupUi(this);
}

Version2::~Version2()
{
    delete ui;
}

void Version2::showEvent(QShowEvent *event){
    Q_UNUSED(event);

    Sequence::getPtr()->setTitle("setup_about");
    if (ExGlobal::getPtr()->getCaliParam("MachineMode")==0)
        ui->label_machine_type_2->setText("FDx-500");
    else
        ui->label_machine_type_2->setText("FDx-1000");

    ui->label_soft_v_2->setText(ExGlobal::getPtr()->version());
    ui->label_soft_v_all_2->setText(ExGlobal::getPtr()->buildversion());
    ui->label_mac_address_2->setText(ExGlobal::getPtr()->getMac());
    ui->label_serail_2->setText(ExGlobal::getPtr()->getSerial());
}

void Version2::hideEvent(QHideEvent *event){
    Q_UNUSED(event);

}
