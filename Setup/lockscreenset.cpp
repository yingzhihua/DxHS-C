#include "lockscreenset.h"
#include "ui_lockscreenset.h"
#include "../module/exglobal.h"
#include "../module/uihandler.h"

LockScreenSet::LockScreenSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LockScreenSet)
{
    ui->setupUi(this);

    ui->label_background->setGeometry(668,290,584,306);
    ui->label_switch->setGeometry(717,319,257,42);    
    ui->pushButton_switch->setGeometry(1099,312,90,58);    
    ui->label_time->setGeometry(718,470,164,42);
    ui->comboBox_time->setGeometry(910,456,293,71);
    ui->pushButton_return->setGeometry(1450,754,299,106);

}

LockScreenSet::~LockScreenSet()
{
    delete ui;
}

void LockScreenSet::showEvent(QShowEvent *event){
    Q_UNUSED(event);

    if (ExGlobal::getPtr()->lockscreenOpen())
    {
      //ui->pushButton_switch->setStyleSheet("background-image: url(:/images/lockscreenON.png);background-color:rgba(255,255,255,100);");
      ui->pushButton_switch->setStyleSheet("background-image: url(:/images/lockscreenON.png);background-color:rgba(209,209,209,100);");
      ui->comboBox_time->setCurrentIndex(ExGlobal::getPtr()->lockTime());
      ui->comboBox_time->setEnabled(true);
    }
    else
    {
      ui->pushButton_switch->setStyleSheet("background-image: url(:/images/lockcreenOFF.png);background-color:rgba(209,209,209,100);");
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
         ui->pushButton_switch->setStyleSheet("background-image: url(:/images/lockcreenOFF.png);background-color:rgba(209,209,209,100);");
    }
    else
    {
        ExGlobal::getPtr()->setLockscreenOpen(true);
        ui->comboBox_time->setEnabled(true);
        ui->pushButton_switch->setStyleSheet("background-image: url(:/images/lockscreenON.png);background-color:rgba(209,209,209,100);");
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
