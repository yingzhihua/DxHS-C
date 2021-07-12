#include "login.h"
#include "ui_login.h"

#include "module/uihandler.h"
#include "module/exglobal.h"
#include "module/sequence.h"

#include <QDebug>
#include <QListView>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);       
    ui->Home_Login_lbBG->setGeometry((ExGlobal::contentWidth-811)/2,(ExGlobal::contentHeight-485)/2,811,485);

    QImage logo(QString(":/images/loginlogo.png"));
    ui->Home_Login_lbLogo->setGeometry((ExGlobal::contentWidth-272)/2,ui->Home_Login_lbBG->y()+49,272,64);
    ui->Home_Login_cbName->setGeometry((ExGlobal::contentWidth-640)/2,ui->Home_Login_lbBG->y()+141,640,71);
    ui->Home_Login_lePassword->setGeometry(ui->Home_Login_cbName->x(),ui->Home_Login_lbBG->y()+235,640,70);
    ui->Home_Login_btLogin->setGeometry((ExGlobal::contentWidth-315)/2,ui->Home_Login_lbBG->y()+347,315,91);
    ui->Home_Login_cbName->setView(new QListView);
}

Login::~Login()
{
    delete ui;
}

void Login::showEvent(QShowEvent *event){
    Q_UNUSED(event);
    ui->Home_Login_cbName->clear();
    ui->Home_Login_cbName->addItems(ExGlobal::pUserModel->getLogUser());
    ui->Home_Login_lePassword->clear();
    Sequence::getPtr()->footerNotify(false,false,false);
}

void Login::hideEvent(QHideEvent *event){
    Q_UNUSED(event);
    qDebug()<<"login hideEvent";
}

void Login::on_Home_Login_btLogin_clicked()
{    
    if (ExGlobal::pUserModel->login(ui->Home_Login_cbName->currentText(),ui->Home_Login_lePassword->text()) == 0)
    {
        UIHandler::UpdateState(UIHandler::StateId::State_User_Update);
        UIHandler::GoPage(UIHandler::PageId::Page_Main_Idle);        
    }
}
