#include "projectparam.h"
#include "ui_projectparam.h"

ProjectParam::ProjectParam(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProjectParam)
{
    ui->setupUi(this);
}

ProjectParam::~ProjectParam()
{
    delete ui;
}
