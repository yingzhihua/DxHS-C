#include "timeset.h"
#include "ui_timeset.h"

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

void TimeSet::on_calendarWidget_clicked(const QDate &date)
{
   // QDateTime timeNew;
   // timeNew.
    ui->dateTimeEdit->setDate(date);

}
