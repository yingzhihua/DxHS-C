#include "testprocess.h"
#include "ui_testprocess.h"

TestProcess::TestProcess(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestProcess)
{
    ui->setupUi(this);
}

TestProcess::~TestProcess()
{
    delete ui;
}
