#include "network.h"
#include "ui_network.h"

NetWork::NetWork(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NetWork)
{
    ui->setupUi(this);
}

NetWork::~NetWork()
{
    delete ui;
}
