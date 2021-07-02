#include "timeset.h"
#include "ui_timeset.h"

#include "../module/uihandler.h"
#include "../module/sequence.h"
#include "../module/exglobal.h"

TimeSet::TimeSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimeSet)
{
    ui->setupUi(this);

}

TimeSet::~TimeSet()
{
    delete ui;
}

void TimeSet::showEvent(QShowEvent *event){
    Q_UNUSED(event);



    QDateTime curDateTime=QDateTime::currentDateTime();

    ui->dTEdit_time->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    ui->dTEdit_time->setDateTime(QDateTime::currentDateTime());
    ui->calendarWidget->selectedDate().setDate(curDateTime.date().year(),curDateTime.date().month(),curDateTime.date().day());
    ui->comboBox->setCurrentIndex(ExGlobal::getPtr()->timezone());
}

void TimeSet::hideEvent(QHideEvent *event){
    Q_UNUSED(event);

}


void TimeSet::on_calendarWidget_clicked(const QDate &date)
{
   // QDateTime timeNew;
   // timeNew.
   // ui->dateTimeEdit->setDate(date);
    ui->dTEdit_time->setDate(date);

}

void TimeSet::on_pushButton_2_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}

void TimeSet::on_pushButton_3_clicked()
{
    QString setStr;
    setStr=ui->dTEdit_time->dateTime().toString("yyyy-MM-dd hh:mm");
    qDebug()<<"dateTime:="<<setStr;
     ExGlobal::getPtr()->setTime(setStr);
}

void TimeSet::on_pushButton_clicked()
{
    if (ExGlobal::getPtr()->timezoneopen())
    {
        ExGlobal::getPtr()->settimezoneopen(false);
       //  ui->comboBox->setEnabled(false);
         ui->pushButton->setStyleSheet("background-image: url(:/images/lockcreenOFF.png);");
    }
    else
    {
        ExGlobal::getPtr()->settimezoneopen(true);
       // ui->comboBox->setEnabled(true);
        ui->pushButton->setStyleSheet("background-image: url(:/images/lockscreenON.png);");
    }
}

void TimeSet::on_comboBox_currentIndexChanged(int index)
{
    ExGlobal::getPtr()->settimezone(index);
}
