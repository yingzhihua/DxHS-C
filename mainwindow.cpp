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
    
    ui->stackedWidget->setCurrentWidget(start);    
}

void MainWindow::UISetup()
{
    const static int HEADER_HEIGHT = 150;
    const static int FOOTER_HEIGHT = 150;
    int screenwidth = QGuiApplication::screens().at(0)->geometry().width();
    int screenheight = QGuiApplication::screens().at(0)->geometry().height();
    qDebug()<<"width="<<screenwidth<<"height="<<screenheight;
    ui->lbHeaderBottom->setGeometry(0,0,screenwidth,HEADER_HEIGHT);
    ui->lbHeaderTop->setGeometry(0,0,screenwidth,HEADER_HEIGHT);
    ui->btSetup->setGeometry(0,screenheight-FOOTER_HEIGHT,screenwidth/3,FOOTER_HEIGHT);
    ui->btHome->setGeometry(ui->btSetup->geometry().x()+ui->btSetup->geometry().width(),screenheight-FOOTER_HEIGHT,screenwidth/3,FOOTER_HEIGHT);
    ui->btData->setGeometry(ui->btHome->geometry().x()+ui->btHome->geometry().width(),screenheight-FOOTER_HEIGHT,screenwidth/3,FOOTER_HEIGHT);
    ui->stackedWidget->setGeometry(0,HEADER_HEIGHT,screenwidth,screenheight-HEADER_HEIGHT-FOOTER_HEIGHT);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::GoPage(UIHandler::PageId id)
{
    qDebug()<<"main GoPage,id="<<id;
    if (id == UIHandler::PageId::Page_Main_Login){
        if (ui->stackedWidget->currentWidget() != main_login){
            main_login->Init();
            ui->stackedWidget->setCurrentWidget(main_login);
        }
    }
    else if(id == UIHandler::PageId::Page_Main_Idle){
        if (ui->stackedWidget->currentWidget() != main_idle){
            ui->stackedWidget->setCurrentWidget(main_idle);
        }
    }
    else if(id == UIHandler::PageId::Page_Main){
        if (ui->stackedWidget->currentWidget() != main_idle){
            ui->stackedWidget->setCurrentWidget(main_idle);
        }
    }
    else if(id == UIHandler::PageId::Page_Setup){
        if (ui->stackedWidget->currentWidget() != setup_menu){
            ui->stackedWidget->setCurrentWidget(setup_menu);
        }
    }
    else if(id == UIHandler::PageId::Page_Data){
        if (ui->stackedWidget->currentWidget() != data_menu){
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
