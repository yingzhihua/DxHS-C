#include "login.h"
#include "ui_login.h"

#include "module/uihandler.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::Init()
{
    
}

void Login::on_btLogin_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Main_Idle);
}
