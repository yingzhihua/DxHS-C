#include "adminpassword.h"
#include "ui_adminpassword.h"

AdminPassword::AdminPassword(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdminPassword)
{
    ui->setupUi(this);
}

AdminPassword::~AdminPassword()
{
    delete ui;
}
