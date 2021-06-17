#include "lockscreenset.h"
#include "ui_lockscreenset.h"
#include "../module/exglobal.h"
#include "../module/uihandler.h"

LockScreenSet::LockScreenSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LockScreenSet)
{
    ui->setupUi(this);

   //  ui->pushButton->setWindowFilePath(":/images/lockscreenON.png");
   // ui->pushButton->setStyleSheet(":/images/lockscreenON.png");
    // ui->pushButton->setStyleSheet(":/images/lockcreenOFF.png");
    if (ExGlobal::getPtr()->lockscreenOpen())
    {
      ui->pushButton->setStyleSheet("background-image: url(:/images/lockscreenON.png);");
      ui->comboBox->setCurrentIndex(ExGlobal::getPtr()->lockTime());
    }
    else
        ui->pushButton->setStyleSheet("background-image: url(:/images/lockcreenOFF.png);");
}

LockScreenSet::~LockScreenSet()
{
    delete ui;
}

void LockScreenSet::on_pushButton_clicked()
{
    if (ExGlobal::getPtr()->lockscreenOpen())
    {
        ExGlobal::getPtr()->setLockscreenOpen(false);
         ui->comboBox->setEnabled(false);
         ui->pushButton->setStyleSheet("background-image: url(:/images/lockcreenOFF.png);");
    }
    else
    {
        ExGlobal::getPtr()->setLockscreenOpen(true);
        ui->comboBox->setEnabled(true);
        ui->pushButton->setStyleSheet("background-image: url(:/images/lockscreenON.png);");
    }
}

void LockScreenSet::on_comboBox_currentIndexChanged(int index)
{
    if (ExGlobal::getPtr()->lockTime() != index)
        ExGlobal::getPtr()->setLockTime(index);
}

void LockScreenSet::on_pushButton_2_clicked()
{
     UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}
