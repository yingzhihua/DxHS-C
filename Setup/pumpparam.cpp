#include "pumpparam.h"
#include "ui_pumpparam.h"

PumpParam::PumpParam(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PumpParam)
{
    ui->setupUi(this);
}

PumpParam::~PumpParam()
{
    delete ui;
}
