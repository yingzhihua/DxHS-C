#include "lockscreenset.h"
#include "ui_lockscreenset.h"

LockScreenSet::LockScreenSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LockScreenSet)
{
    ui->setupUi(this);
}

LockScreenSet::~LockScreenSet()
{
    delete ui;
}
