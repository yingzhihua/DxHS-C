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
    ui->label_soft_v->setGeometry(500,100,300,60);
    ui->label_soft_v_2->setGeometry(800,100,500,60);
    ui->label->setGeometry(500,150,700,60);
    ui->label_soft_v_all->setGeometry(500,200,300,60);
    ui->label_soft_v_all_2->setGeometry(800,200,500,60);
    ui->label_2->setGeometry(500,250,700,60);
    ui->label_machine_type->setGeometry(500,300,300,60);
    ui->label_machine_type_2->setGeometry(800,300,500,60);
    ui->label_3->setGeometry(500,350,700,60);
    ui->label_mac_address->setGeometry(500,400,300,60);
    ui->label_mac_address_2->setGeometry(800,400,500,60);
    ui->label_4->setGeometry(500,450,700,60);
    ui->label_serail->setGeometry(500,500,300,60);
    ui->label_serail_2->setGeometry(680,500,500,60);

    ui->pushButton->setGeometry(1580,740,300,115);
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

void Version2::on_pushButton_clicked()
{
  UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}
