#include "wifimenu.h"
#include "ui_wifimenu.h"

WifiMenu::WifiMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WifiMenu)
{
    ui->setupUi(this);
}

WifiMenu::~WifiMenu()
{
    delete ui;
}
