#include "systemparam.h"
#include "ui_systemparam.h"

SystemParam::SystemParam(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SystemParam)
{
    ui->setupUi(this);
}

SystemParam::~SystemParam()
{
    delete ui;
}
