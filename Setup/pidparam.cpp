#include "pidparam.h"
#include "ui_pidparam.h"

PIDParam::PIDParam(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PIDParam)
{
    ui->setupUi(this);
}

PIDParam::~PIDParam()
{
    delete ui;
}
