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




    ui->pushButton_3->setGeometry(1450,624,299,106);
    ui->pushButton_2->setGeometry(1450,754,299,106);
    ui->calendarWidget->setGeometry(378,147,698,610);
    ui->label->setGeometry(1200,147,220,45);
    ui->pushButton->setGeometry(1430,147,90,58);
    ui->label_2->setGeometry(1200,277,220,45);
    ui->comboBox->setGeometry(1430,257,400,80);
    ui->label_3->setGeometry(1200,397,220,45);
    ui->dTEdit_time->setGeometry(1430,367,450,80);
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
