#include "version2.h"
#include "ui_version2.h"

Version2::Version2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Version2)
{
    ui->setupUi(this);
}

Version2::~Version2()
{
    delete ui;
}
