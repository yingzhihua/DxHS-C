#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QScreen>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    global = ExGlobal::getPtr();
    SqliteMgr::conn(Log::getDir()+"/data.db","sa","123456");
    ExGlobal::exInit();

    sequence = Sequence::getPtr();
    connect(UIHandler::getPtr(),&UIHandler::Go,this,&MainWindow::GoPage);

    ui->setupUi(this);
    UISetup();

    start = new Startup(this);    
    ui->stackedWidget->addWidget(start);
    main_login = new Login(this);
    ui->stackedWidget->addWidget(main_login);
    main_idle = new Idle(this);
    ui->stackedWidget->addWidget(main_idle);

    setup_menu = new SetupMenu(this);
    ui->stackedWidget->addWidget(setup_menu);
    data_menu = new DataMenu(this);
    ui->stackedWidget->addWidget(data_menu);
    
    if (ExGlobal::isDebug())
        ui->stackedWidget->setCurrentWidget(main_login);
    else
        ui->stackedWidget->setCurrentWidget(start);
    this->startTimer(1000);
}

void MainWindow::UISetup()
{
    const static int HEADER_HEIGHT = 150;
    const static int FOOTER_HEIGHT = 150;
    int screenwidth = QGuiApplication::screens().at(0)->geometry().width();
    int screenheight = QGuiApplication::screens().at(0)->geometry().height();
    qDebug()<<"width="<<screenwidth<<"height="<<screenheight;
    ExGlobal::contentWidth = screenwidth;
    ExGlobal::contentHeight = screenheight-HEADER_HEIGHT-FOOTER_HEIGHT;

    ui->lbHeaderBottom->setGeometry(0,0,screenwidth,HEADER_HEIGHT);
    ui->lbHeaderTop->setGeometry(0,0,screenwidth,HEADER_HEIGHT);
    ui->btSetup->setGeometry(0,screenheight-FOOTER_HEIGHT,screenwidth/3,FOOTER_HEIGHT);
    ui->btHome->setGeometry(ui->btSetup->geometry().x()+ui->btSetup->geometry().width(),screenheight-FOOTER_HEIGHT,screenwidth/3,FOOTER_HEIGHT);
    ui->btData->setGeometry(ui->btHome->geometry().x()+ui->btHome->geometry().width(),screenheight-FOOTER_HEIGHT,screenwidth/3,FOOTER_HEIGHT);
    ui->stackedWidget->setGeometry(0,HEADER_HEIGHT,screenwidth,screenheight-HEADER_HEIGHT-FOOTER_HEIGHT);
    ui->lbDate->setGeometry(1540,HEADER_HEIGHT+30,300,60);
    ui->lbUser->setGeometry(100,HEADER_HEIGHT+30,300,60);

    ui->lbTitleIcon->setGeometry(69,42,66,66);
    ui->lbTitle->setGeometry(160,50,1000,50);
    ui->lbMachineName->setGeometry(1300,50,500,50);
    ui->lbMachineName->setAlignment(Qt::AlignRight);
    ui->lbMachineName->setText(ExGlobal::getPtr()->sysName());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *e)
{
    ui->lbDate->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::GoPage(UIHandler::PageId id)
{
    qDebug()<<"main GoPage,id="<<id;
    if (id == UIHandler::PageId::Page_Main_Login){        
        if (ui->stackedWidget->currentWidget() != main_login){
            ui->lbDate->setVisible(true);
            ui->lbUser->setVisible(true);
            ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/title_login.png")));
            ui->lbTitle->setText("登录");
            main_login->Init();
            ui->stackedWidget->setCurrentWidget(main_login);
        }
    }
    else if(id == UIHandler::PageId::Page_Main_Idle){
        if (ui->stackedWidget->currentWidget() != main_idle){
            ui->lbDate->setVisible(true);
            ui->lbUser->setVisible(true);
            ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/title_idle.png")));
            ui->lbTitle->setText("待机");
            ui->stackedWidget->setCurrentWidget(main_idle);
        }
    }
    else if(id == UIHandler::PageId::Page_Main){
        if (ui->stackedWidget->currentWidget() != main_idle){
            ui->lbDate->setVisible(true);
            ui->lbUser->setVisible(true);
            ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/title_idle.png")));
            ui->lbTitle->setText("待机");
            ui->stackedWidget->setCurrentWidget(main_idle);
        }
    }
    else if(id == UIHandler::PageId::Page_Setup){
        if (ui->stackedWidget->currentWidget() != setup_menu){
            ui->lbDate->setVisible(false);
            ui->lbUser->setVisible(false);
            ui->lbTitle->setText("设置");
            ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/title_setup.png")));
            setup_menu->Init();
            ui->stackedWidget->setCurrentWidget(setup_menu);
        }
    }
    else if(id == UIHandler::PageId::Page_Data){
        if (ui->stackedWidget->currentWidget() != data_menu){
            ui->lbDate->setVisible(false);
            ui->lbUser->setVisible(false);
            ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/title_datamenu.png")));
            ui->lbTitle->setText("历史数据");
            ui->stackedWidget->setCurrentWidget(data_menu);
        }
    }
}

void MainWindow::on_btSetup_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}

void MainWindow::on_btHome_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Main);
}

void MainWindow::on_btData_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Data);
}
