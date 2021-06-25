#include <QDebug>
#include <QScreen>
#include <QMovie>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    global = ExGlobal::getPtr();
    SqliteMgr::conn(Log::getDir()+"/data.db","sa","123456");
    ExGlobal::exInit();

    sequence = Sequence::getPtr();
    connect(sequence,&Sequence::footerNotify,this,&MainWindow::FooterNotify);
    connect(sequence,&Sequence::titleNotify,this,&MainWindow::TitleNotify);
    connect(UIHandler::getPtr(),&UIHandler::Go,this,&MainWindow::GoPage);
    connect(UIHandler::getPtr(),&UIHandler::State,this,&MainWindow::StateUpdate);

    ui->setupUi(this);    
    UISetup();

    start = new Startup(this);    
    ui->stackedWidget->addWidget(start);
    main_login = new Login(this);
    ui->stackedWidget->addWidget(main_login);
    main_idle = new Idle(this);
    ui->stackedWidget->addWidget(main_idle);
    main_boxready = new BoxReady(this);
    ui->stackedWidget->addWidget(main_boxready);
    main_testprocess = new TestProcess(this);
    ui->stackedWidget->addWidget(main_testprocess);
    setup_menu = new SetupMenu(this);
    ui->stackedWidget->addWidget(setup_menu);
    data_menu = new DataMenu(this);
    ui->stackedWidget->addWidget(data_menu);
    
    systemname = new SystemName(this);
    ui->stackedWidget->addWidget(systemname);

    backlight = new BackLight(this);
    ui->stackedWidget->addWidget(backlight);

    languageset = new LanguageSet(this);
    ui->stackedWidget->addWidget(languageset);

    adminpassword = new AdminPassword(this);
    ui->stackedWidget->addWidget(adminpassword);

    lockscreenset = new LockScreenSet(this);
    ui->stackedWidget->addWidget(lockscreenset);

    wifimenu = new WifiMenu(this);
    ui->stackedWidget->addWidget(wifimenu);

    network = new NetWork(this);
    ui->stackedWidget->addWidget(network);

    timeset = new TimeSet(this);
    ui->stackedWidget->addWidget(timeset);

    user = new User(this);
    ui->stackedWidget->addWidget(user);

    machinecalibration = new MachineCalibration(this);
    ui->stackedWidget->addWidget(machinecalibration);

    camerasetup = new CameraSetup(this);
    ui->stackedWidget->addWidget(camerasetup);

    testsetup = new TestSetup(this);
    ui->stackedWidget->addWidget(testsetup);

    testloop = new TestLoop(this);
    ui->stackedWidget->addWidget(testloop);

    systemparam = new SystemParam(this);
    ui->stackedWidget->addWidget(systemparam);

    projectparam = new ProjectParam(this);
    ui->stackedWidget->addWidget(projectparam);

    usbfile = new UsbFile(this);
    ui->stackedWidget->addWidget(usbfile);

    light = new Light(this);
    ui->stackedWidget->addWidget(light);

    logview = new LogView(this);
    ui->stackedWidget->addWidget(logview);

    pidparam = new PIDParam(this);
    ui->stackedWidget->addWidget(pidparam);

    pumpparam = new PumpParam(this);
    ui->stackedWidget->addWidget(pumpparam);

    version2 = new Version2(this);
    ui->stackedWidget->addWidget(version2);

    adduser = new AddUser(this);
    ui->stackedWidget->addWidget(adduser);

    updateuser = new UpdateUser(this);
    ui->stackedWidget->addWidget(updateuser);



    if (ExGlobal::isDebug())
        ui->stackedWidget->setCurrentWidget(main_login);
        //ui->stackedWidget->setCurrentWidget(main_testprocess);
    else
        ui->stackedWidget->setCurrentWidget(start);
    this->startTimer(1000);
}

const static int HEADER_HEIGHT = 150;
const static int FOOTER_HEIGHT = 150;
const static int BOTTOMBUTTON_HEIGHT = 123;
const static int BOTTOM_BUTTON_WIDTH = 342;
void MainWindow::UISetup()
{
    int screenwidth = QGuiApplication::screens().at(0)->geometry().width();
    int screenheight = QGuiApplication::screens().at(0)->geometry().height();
    qDebug()<<"width="<<screenwidth<<"height="<<screenheight;
    ExGlobal::contentWidth = screenwidth;
    ExGlobal::contentHeight = screenheight-HEADER_HEIGHT-FOOTER_HEIGHT;

    ui->Home_Main_lbUnfinish->setGeometry(0,0,0,HEADER_HEIGHT);
    ui->lbHeaderTop->setGeometry(0,0,screenwidth,HEADER_HEIGHT);
    ui->lbfooterbg->setGeometry(0,screenheight-FOOTER_HEIGHT,screenwidth,FOOTER_HEIGHT);

    ui->btSetup->setGeometry(161,screenheight-FOOTER_HEIGHT+14,BOTTOM_BUTTON_WIDTH,BOTTOMBUTTON_HEIGHT);
    ui->btSetup->setStyleSheet("QPushButton {font-size:60px;padding-left:30;color:#a7a7a7;background: url(:/images/setuprelease.png)}");
    ui->btHome->setGeometry(801,screenheight-FOOTER_HEIGHT+14,BOTTOM_BUTTON_WIDTH,BOTTOMBUTTON_HEIGHT);
    ui->btHome->setStyleSheet("QPushButton {font-size:60px;padding-left:30;color:#ffffff;background: url(:/images/homepress.png);background-repeat:no-repeat}");
    ui->btData->setGeometry(1440,screenheight-FOOTER_HEIGHT+14,BOTTOM_BUTTON_WIDTH,BOTTOMBUTTON_HEIGHT);;
    ui->btData->setStyleSheet("QPushButton {font-size:60px;padding-left:30;color:#a7a7a7;background: url(:/images/datarelease.png)}");

    ui->stackedWidget->setGeometry(0,HEADER_HEIGHT,screenwidth,screenheight-HEADER_HEIGHT-FOOTER_HEIGHT);
    ui->lbDate->setGeometry(1540,HEADER_HEIGHT+30,300,60);
    ui->lbUser->setGeometry(100,HEADER_HEIGHT+30,300,60);

    ui->lbTitleIcon->setGeometry(69,42,66,66);
    ui->lbTitle->setGeometry(160,50,1000,50);
    ui->lbMachineName->setGeometry(1300,50,500,50);
    ui->lbMachineName->setAlignment(Qt::AlignRight);
    ui->lbMachineName->setText(ExGlobal::getPtr()->sysName());
    qDebug()<<"sysName"<<ExGlobal::getPtr()->sysName();

    ui->lbLoading->setGeometry(0,0,screenwidth,screenheight);
    ui->lbLoading->setAlignment(Qt::AlignCenter);
    movie = new QMovie(":/images/loading.gif");
    ui->lbLoading->setMovie(movie);
    ui->lbLoading->setVisible(false);

    FooterNotify(false,false,false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *e)
{
    Q_UNUSED(e);
    ui->lbDate->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
}

void MainWindow::GoPage(UIHandler::PageId id)
{
    qDebug()<<"main GoPage,id="<<id<<ui->stackedWidget->currentWidget()->objectName();

    if (id == UIHandler::PageId::Page_Main_Login){
        if (ui->stackedWidget->currentWidget() != main_login){
            ui->lbDate->setVisible(true);
            ui->lbUser->setVisible(true);
            ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/title_login.png")));
            ui->lbTitle->setText("登录");            
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
    else if(id == UIHandler::PageId::Page_Main_BoxReady){
        if (ui->stackedWidget->currentWidget() != main_boxready){
            ui->lbDate->setVisible(true);
            ui->lbUser->setVisible(true);
            ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/title_boxready.png")));
            ui->lbTitle->setText(tr("试剂盒就绪"));
            ui->stackedWidget->setCurrentWidget(main_boxready);
        }
    }
    else if(id == UIHandler::PageId::Page_Main_Test){
        if (ui->stackedWidget->currentWidget() != main_testprocess){
            ui->lbDate->setVisible(true);
            ui->lbUser->setVisible(true);
            ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/title_test.png")));
            ui->lbTitle->setText(tr("测试中"));
            ui->stackedWidget->setCurrentWidget(main_testprocess);
        }
    }
    else if(id == UIHandler::PageId::Page_Main){
        if (ui->stackedWidget->currentWidget() != main_idle && ui->stackedWidget->currentWidget() != main_boxready){
            ui->btSetup->setStyleSheet("QPushButton {font-size:60px;padding-left:30;color:#a7a7a7;background: url(:/images/setuprelease.png);background-repeat:no-repeat}");
            ui->btHome->setStyleSheet("QPushButton {font-size:60px;padding-left:30;color:#ffffff;background: url(:/images/homepress.png);background-repeat:no-repeat}");
            ui->btData->setStyleSheet("QPushButton {font-size:60px;padding-left:30;color:#a7a7a7;background: url(:/images/datarelease.png);background-repeat:no-repeat}");
            if (Sequence::getPtr()->readStage() == Sequence::StageState::Stage_idle)
                UIHandler::GoPage(UIHandler::PageId::Page_Main_Idle);
            else
                UIHandler::GoPage(UIHandler::PageId::Page_Main_BoxReady);
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
            ui->btSetup->setStyleSheet("QPushButton {font-size:60px;padding-left:30;color:#ffffff;background: url(:/images/setuppress.png);background-repeat:no-repeat}");
            ui->btHome->setStyleSheet("QPushButton {font-size:60px;padding-left:30;color:#a7a7a7;background: url(:/images/homerelease.png);background-repeat:no-repeat}");
            ui->btData->setStyleSheet("QPushButton {font-size:60px;padding-left:30;color:#a7a7a7;background: url(:/images/datarelease.png);background-repeat:no-repeat}");
        }
    }
    else if(id == UIHandler::PageId::Page_Data){
        if (ui->stackedWidget->currentWidget() != data_menu){
            ui->lbDate->setVisible(false);
            ui->lbUser->setVisible(false);
            ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/title_datamenu.png")));
            ui->lbTitle->setText("历史数据");
            ui->stackedWidget->setCurrentWidget(data_menu);
            ui->btSetup->setStyleSheet("QPushButton {font-size:60px;padding-left:30;color:#a7a7a7;background: url(:/images/setuprelease.png);background-repeat:no-repeat}");
            ui->btHome->setStyleSheet("QPushButton {font-size:60px;padding-left:30;color:#a7a7a7;background: url(:/images/homerelease.png);background-repeat:no-repeat}");
            ui->btData->setStyleSheet("QPushButton {font-size:60px;padding-left:30;color:#ffffff;background: url(:/images/datapress.png);background-repeat:no-repeat}");
        }
    }
    else if(id == UIHandler::PageId::Page_Setup_SystemName){
        if (ui->stackedWidget->currentWidget() != systemname){
            ui->lbDate->setVisible(false);
            ui->lbUser->setVisible(false);
            ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
            ui->lbTitle->setText("系统参数");
            ui->stackedWidget->setCurrentWidget(systemname);
        }
    }

    else if(id == UIHandler::PageId::Page_Setup_Backlight){
                if (ui->stackedWidget->currentWidget() != backlight){
                    ui->lbDate->setVisible(false);
                    ui->lbUser->setVisible(false);
                    ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                    ui->lbTitle->setText("背光设置");
                    ui->stackedWidget->setCurrentWidget(backlight);
                }
            }

      else if(id == UIHandler::PageId::Page_Setup_Language){
                        if (ui->stackedWidget->currentWidget() != languageset){
                            ui->lbDate->setVisible(false);
                            ui->lbUser->setVisible(false);
                            ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                            ui->lbTitle->setText("语言设置");
                            ui->stackedWidget->setCurrentWidget(languageset);
                        }
                    }
    else if(id == UIHandler::PageId::Page_Setup_Password){
                      if (ui->stackedWidget->currentWidget() != adminpassword){
                          ui->lbDate->setVisible(false);
                          ui->lbUser->setVisible(false);
                          ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                          ui->lbTitle->setText("修改密码");
                          ui->stackedWidget->setCurrentWidget(adminpassword);
                      }
                  }

    else if(id == UIHandler::PageId::Page_Setup_LockScreen){
                      if (ui->stackedWidget->currentWidget() != lockscreenset){
                          ui->lbDate->setVisible(false);
                          ui->lbUser->setVisible(false);
                          ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                          ui->lbTitle->setText("锁屏时间");
                          ui->stackedWidget->setCurrentWidget(lockscreenset);
                      }
                  }
    else if(id == UIHandler::PageId::Page_Setup_Wifi){
                      if (ui->stackedWidget->currentWidget() != wifimenu){
                          ui->lbDate->setVisible(false);
                          ui->lbUser->setVisible(false);
                          ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                          ui->lbTitle->setText("wifi设置");
                          ui->stackedWidget->setCurrentWidget(wifimenu);
                      }
                  }
    else if(id == UIHandler::PageId::Page_Setup_Network){
                      if (ui->stackedWidget->currentWidget() != network){
                          ui->lbDate->setVisible(false);
                          ui->lbUser->setVisible(false);
                          ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                          ui->lbTitle->setText("网络设置");
                          ui->stackedWidget->setCurrentWidget(network);
                      }
                  }

    else if(id == UIHandler::PageId::Page_Setup_Time){
                      if (ui->stackedWidget->currentWidget() != timeset){
                          ui->lbDate->setVisible(false);
                          ui->lbUser->setVisible(false);
                          ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                          ui->lbTitle->setText("时间设置");
                          ui->stackedWidget->setCurrentWidget(timeset);
                      }
                  }

    else if(id == UIHandler::PageId::Page_Setup_User){
                      if (ui->stackedWidget->currentWidget() != user){
                          user->load_data();
                          ui->lbDate->setVisible(false);
                          ui->lbUser->setVisible(false);
                          ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                          ui->lbTitle->setText("用户设置");
                          ui->stackedWidget->setCurrentWidget(user);
                      }
                  }

    else if(id == UIHandler::PageId::Page_Setup_Machine){
                      if (ui->stackedWidget->currentWidget() != machinecalibration){
                          ui->lbDate->setVisible(false);
                          ui->lbUser->setVisible(false);
                          ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                          ui->lbTitle->setText("机械校准");
                          ui->stackedWidget->setCurrentWidget(machinecalibration);
                      }
                  }

    else if(id == UIHandler::PageId::Page_Setup_Camera){
                      if (ui->stackedWidget->currentWidget() != camerasetup){
                          camerasetup->load_data();
                          ui->lbDate->setVisible(false);
                          ui->lbUser->setVisible(false);
                          ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                          ui->lbTitle->setText("摄像头参数设置");
                          ui->stackedWidget->setCurrentWidget(camerasetup);
                      }
                  }

     else if(id == UIHandler::PageId::Page_Setup_TestSetup){
                      if (ui->stackedWidget->currentWidget() != testsetup){
                          testsetup->load_data();
                          ui->lbDate->setVisible(false);
                          ui->lbUser->setVisible(false);
                          ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                          ui->lbTitle->setText("测试设置");
                          ui->stackedWidget->setCurrentWidget(testsetup);
                      }
                  }

    else if(id == UIHandler::PageId::Page_Setup_TestLoop){
                     if (ui->stackedWidget->currentWidget() != testloop){
                         testloop->load_data();
                         ui->lbDate->setVisible(false);
                         ui->lbUser->setVisible(false);
                         ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                         ui->lbTitle->setText("循环测试");
                         ui->stackedWidget->setCurrentWidget(testloop);
                     }
                 }
    else if(id == UIHandler::PageId::Page_Setup_SystemParam){
                     if (ui->stackedWidget->currentWidget() != systemparam){
                         ui->lbDate->setVisible(false);
                         ui->lbUser->setVisible(false);
                         ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                         ui->lbTitle->setText("系统参数");
                         ui->stackedWidget->setCurrentWidget(systemparam);
                     }
                 }
    else if(id == UIHandler::PageId::Page_Setup_ProjectParam){
                     if (ui->stackedWidget->currentWidget() != projectparam){
                         projectparam->load_data();
                         ui->lbDate->setVisible(false);
                         ui->lbUser->setVisible(false);
                         ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                         ui->lbTitle->setText("工程参数");
                         ui->stackedWidget->setCurrentWidget(projectparam);
                     }
                 }

    else if(id == UIHandler::PageId::Page_Setup_UsbFile){
                     if (ui->stackedWidget->currentWidget() != usbfile){
                         ui->lbDate->setVisible(false);
                         ui->lbUser->setVisible(false);
                         ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                         ui->lbTitle->setText("数据维护");
                         ui->stackedWidget->setCurrentWidget(usbfile);
                     }
                 }

     else if(id == UIHandler::PageId::Page_Setup_Light){
                     if (ui->stackedWidget->currentWidget() != light){
                         light->load_data();
                         ui->lbDate->setVisible(false);
                         ui->lbUser->setVisible(false);
                         ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                         ui->lbTitle->setText("荧光强度");
                         ui->stackedWidget->setCurrentWidget(light);
                     }
                 }

    else if(id == UIHandler::PageId::Page_Setup_PumpParam){
                    if (ui->stackedWidget->currentWidget() != pumpparam){
                        ui->lbDate->setVisible(false);
                        ui->lbUser->setVisible(false);
                        ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                        ui->lbTitle->setText("流体参数");
                        ui->stackedWidget->setCurrentWidget(pumpparam);
                    }
                }
    else if(id == UIHandler::PageId::Page_Setup_Log){
                    if (ui->stackedWidget->currentWidget() != logview){
                        logview->display_data();
                        ui->lbDate->setVisible(false);
                        ui->lbUser->setVisible(false);
                        ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                        ui->lbTitle->setText("LOG信息");
                        ui->stackedWidget->setCurrentWidget(logview);
                    }
                }
    else if(id == UIHandler::PageId::Page_Setup_PID){
                    if (ui->stackedWidget->currentWidget() != pidparam){
                        ui->lbDate->setVisible(false);
                        ui->lbUser->setVisible(false);
                        ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                        ui->lbTitle->setText("PID参数");
                        ui->stackedWidget->setCurrentWidget(pidparam);
                    }
                }
    else if(id == UIHandler::PageId::Page_Setup_Version){
                    if (ui->stackedWidget->currentWidget() != version2){
                        ui->lbDate->setVisible(false);
                        ui->lbUser->setVisible(false);
                        ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                        ui->lbTitle->setText("版本信息");
                        ui->stackedWidget->setCurrentWidget(version2);
                    }
                }
  else if(id == UIHandler::PageId::Page_User_AddUser){
                    if (ui->stackedWidget->currentWidget() != adduser){
                        ui->lbDate->setVisible(false);
                        ui->lbUser->setVisible(false);
                        ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                        ui->lbTitle->setText("增加用户");
                        ui->stackedWidget->setCurrentWidget(adduser);
                    }
                }
    else if(id == UIHandler::PageId::Page_User_UpdateUser){
                    if (ui->stackedWidget->currentWidget() != updateuser){
                        updateuser->show_data();
                        ui->lbDate->setVisible(false);
                        ui->lbUser->setVisible(false);
                        ui->lbTitleIcon->setPixmap(QPixmap::fromImage(QImage(":/images/setupmenu_about.png")));
                        ui->lbTitle->setText("修改用户");
                        ui->stackedWidget->setCurrentWidget(updateuser);
                    }

  }
}
void MainWindow::StateUpdate(UIHandler::StateId id)
{
    if (id == UIHandler::StateId::State_Loading_Open){
        ui->lbLoading->setVisible(true);
        movie->start();
    }
    else if (id == UIHandler::StateId::State_Loading_Close){
        movie->stop();
        ui->lbLoading->setVisible(false);
    }
    else if (id == UIHandler::StateId::State_User_Update){
        ui->lbUser->setText(ExGlobal::DisplayUser);
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

void MainWindow::FooterNotify(bool setup,bool home,bool data)
{
    ui->btSetup->setEnabled(setup);
    ui->btHome->setEnabled(home);
    ui->btData->setEnabled(data);
}

void MainWindow::TitleNotify(int titleparam, QString title)
{
    qDebug()<<"TitleNotify,"<<titleparam<<title;
    if (titleparam == 0)
        ui->lbTitle->setText(title);
    else if (titleparam == 1)
        ui->lbDate->setVisible(false);
    else if (titleparam == 2)
        ui->lbDate->setVisible(true);
    else if (titleparam >= 100){        
        int x = (titleparam - 100)*ExGlobal::contentWidth/1000;
        ui->Home_Main_lbUnfinish->setGeometry(x,0,ExGlobal::contentWidth-x,HEADER_HEIGHT);
        ui->lbTitle->setText(title);
    }
}
