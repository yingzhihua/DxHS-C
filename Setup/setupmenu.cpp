#include "setupmenu.h"
#include "ui_setupmenu.h"

SetupMenu::SetupMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupMenu)
{
    ui->setupUi(this);
}

SetupMenu::~SetupMenu()
{
    delete ui;
}
