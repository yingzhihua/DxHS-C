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

    ui->label_soft_v->setGeometry(586,264,210,40);
    ui->label_soft_v_2->setGeometry(960,264,400,40);
    ui->label->setGeometry(584,309,780,20);
    ui->label_soft_v_all->setGeometry(586,347,300,40);
    ui->label_soft_v_all_2->setGeometry(960,347,400,40);
    ui->label_2->setGeometry(584,392,780,20);
    ui->label_machine_type->setGeometry(586,430,300,40);
    ui->label_machine_type_2->setGeometry(960,430,400,40);
    ui->label_3->setGeometry(584,475,780,20);
    ui->label_mac_address->setGeometry(586,513,300,40);
    ui->label_mac_address_2->setGeometry(960,513,400,40);
    ui->label_4->setGeometry(584,559,780,20);
    ui->label_serail->setGeometry(586,591,300,45);
    ui->label_serail_2->setGeometry(960,596,400,40);
    ui->label_5->setGeometry(584,642,780,20);
    ui->pushButton->setGeometry(1450,754,299,106);

}

Version2::~Version2()
{
    delete ui;
}

void Version2::showEvent(QShowEvent *event){
    Q_UNUSED(event);

    Sequence::getPtr()->setTitle("setup_about");
    /*
    if (ExGlobal::getPtr()->getCaliParam("MachineMode")==0)
        ui->label_machine_type_2->setText("FDx-500");
    else
        ui->label_machine_type_2->setText("FDx-1000");
*/
    ui->label_soft_v_2->setText(ExGlobal::getPtr()->buildversion());
    ui->label_soft_v_all_2->setText(ExGlobal::getPtr()->tempversion());
    ui->label_machine_type_2->setText(ExGlobal::getPtr()->ctrlversion());
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
