#include "login.h"
#include "ui_login.h"

#include "module/uihandler.h"

#include <QDebug>
Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    QImage bg(QString(":/images/loginbk.png"));
    qDebug()<<bg.width()<<bg.height();
    ui->lbBG->setGeometry((this->width()-bg.width())/2,(this->height()-bg.height())/2,bg.width(),bg.height());
    ui->lbBG->setPixmap(QPixmap::fromImage(bg));

    QImage logo(QString(":/images/loginlogo.png"));
    ui->lbLogo->setGeometry((this->width()-logo.width())/2,ui->lbBG->y()+49,logo.width(),logo.height());
    ui->lbLogo->setPixmap(QPixmap::fromImage(logo));
    ui->tName->setGeometry((this->width()-640)/2,ui->lbLogo->y()+90,640,70);
    ui->tPassword->setGeometry((this->width()-640)/2,ui->tName->y()+90,640,70);

    //ui->btLogin->setStyleSheet("QPushButton{}");
    //QIcon login(QString(":/images/loginbt.png"));
    //ui->btLogin->setGeometry((this->width()-login.)/2,ui->lbBG->y()+49,login.width(),login.height());
    //ui->btLogin->setIcon()
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
