#include "lockscreenset.h"
#include "ui_lockscreenset.h"
#include "../module/exglobal.h"
#include "../module/uihandler.h"

LockScreenSet::LockScreenSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LockScreenSet)
{
    ui->setupUi(this);


    int init_x = (ExGlobal::contentWidth-810)/2;
    int init_y = (ExGlobal::contentHeight-565)/2+150;
    setGeometry(init_x,init_y,810,565);

    ui->label_switch->setGeometry(500,200,200,60);
    ui->pushButton_switch->setGeometry(750,200,500,60);
    ui->label_time->setGeometry(500,300,200,60);
    ui->comboBox_time->setGeometry(750,300,500,60);
    ui->pushButton_return->setGeometry(1580,740,300,115);


}

LockScreenSet::~LockScreenSet()
{
    delete ui;
}

void LockScreenSet::showEvent(QShowEvent *event){
    Q_UNUSED(event);

    if (ExGlobal::getPtr()->lockscreenOpen())
    {
      ui->pushButton_switch->setStyleSheet("background-image: url(:/images/lockscreenON.png);");
      ui->comboBox_time->setCurrentIndex(ExGlobal::getPtr()->lockTime());
      ui->comboBox_time->setEnabled(true);
    }
    else
    {
      ui->pushButton_switch->setStyleSheet("background-image: url(:/images/lockcreenOFF.png);");
      ui->comboBox_time->setEnabled(false);
    }

}

void LockScreenSet::hideEvent(QHideEvent *event){
    Q_UNUSED(event);

}
void LockScreenSet::on_pushButton_clicked()
{
}

void LockScreenSet::on_comboBox_currentIndexChanged(int index)
{

}

void LockScreenSet::on_pushButton_2_clicked()
{
 }

void LockScreenSet::on_pushButton_switch_clicked()
{
    if (ExGlobal::getPtr()->lockscreenOpen())
    {
         ExGlobal::getPtr()->setLockscreenOpen(false);
         ui->comboBox_time->setEnabled(false);
         ui->pushButton_switch->setStyleSheet("background-image: url(:/images/lockcreenOFF.png);");
    }
    else
    {
        ExGlobal::getPtr()->setLockscreenOpen(true);
        ui->comboBox_time->setEnabled(true);
        ui->pushButton_switch->setStyleSheet("background-image: url(:/images/lockscreenON.png);");
    }
}

void LockScreenSet::on_comboBox_time_currentIndexChanged(int index)
{
    if (ExGlobal::getPtr()->lockTime() != index)
        ExGlobal::getPtr()->setLockTime(index);
}

void LockScreenSet::on_pushButton_return_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}
