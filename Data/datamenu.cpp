#include "datamenu.h"
#include "ui_datamenu.h"

DataMenu::DataMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataMenu)
{
    ui->setupUi(this);
}

DataMenu::~DataMenu()
{
    delete ui;
}
