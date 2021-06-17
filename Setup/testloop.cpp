#include "testloop.h"
#include "ui_testloop.h"

TestLoop::TestLoop(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TestLoop)
{
    ui->setupUi(this);
}

TestLoop::~TestLoop()
{
    delete ui;
}
