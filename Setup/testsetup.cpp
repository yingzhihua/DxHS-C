#include "testsetup.h"
#include "ui_testsetup.h"

TestSetup::TestSetup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestSetup)
{
    ui->setupUi(this);
}

TestSetup::~TestSetup()
{
    delete ui;
}
