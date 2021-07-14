#include "setupmenu.h"
#include "ui_setupmenu.h"

#include <QDebug>
#include <QListWidgetItem>
#include <QScrollBar>
#include <QScroller>

#include "../module/exglobal.h"
#include "../module/uihandler.h"

#pragma pack(1)
struct SetupMenuItem
{
    QString Eid;
    QString icon;
    QString name;
    UIHandler::PageId page;
    bool show;
};

static SetupMenuItem item[] = {
    {"SystemName",":/images/setupmenu_system.png", "系统设置", UIHandler::PageId::Page_Setup_SystemName,true},
    {"LockTime",":/images/setupmenu_lock.png", "锁屏时间", UIHandler::PageId::Page_Setup_LockScreen,true},
    {"Blacklight",":/images/setupmenu_system.png", "背光设置", UIHandler::PageId::Page_Setup_Backlight,true},
    {"Language",":/images/setupmenu_lauguage.png", "语言", UIHandler::PageId::Page_Setup_Language,true},
    {"AdminPassword",":/images/setupmenu_password.png", "修改密码", UIHandler::PageId::Page_Setup_Password,true},
    {"Wlan",":/images/setupmenu_wifi.png", "无线网络", UIHandler::PageId::Page_Setup_Wifi,true},
    {"Network",":/images/setupmenu_net.png", "有线网络", UIHandler::PageId::Page_Setup_Network,true},
    {"TimeSet",":/images/setupmenu_time.png", "时间设置", UIHandler::PageId::Page_Setup_Time,true},
    {"User",":/images/setupmenu_user.png", "用户帐号", UIHandler::PageId::Page_Setup_User,true},
    {"Cali",":/images/setupmenu_system.png", "机械校准", UIHandler::PageId::Page_Setup_Machine,true},
    {"Camera",":/images/setupmenu_system.png", "摄像头参数设置", UIHandler::PageId::Page_Setup_Camera,true},
    {"TestSetup",":/images/setupmenu_system.png", "测试设置", UIHandler::PageId::Page_Setup_TestSetup,true},
    {"TestLoop",":/images/setupmenu_system.png", "循环测试", UIHandler::PageId::Page_Setup_TestLoop,true},
    {"SystemParam",":/images/setupmenu_system.png", "系统参数", UIHandler::PageId::Page_Setup_SystemParam,true},
    {"ProjectParam",":/images/setupmenu_system.png", "工程参数", UIHandler::PageId::Page_Setup_ProjectParam,true},
    {"Usb",":/images/setupmenu_system.png", "数据维护", UIHandler::PageId::Page_Setup_UsbFile,true},
    {"Light",":/images/setupmenu_system.png", "荧光强度", UIHandler::PageId::Page_Setup_Light,true},
    {"FluidParam",":/images/setupmenu_system.png", "流体参数", UIHandler::PageId::Page_Setup_PumpParam,true},
    {"Log_view",":/images/setupmenu_system.png", "查看Log信息", UIHandler::PageId::Page_Setup_Log,true},
    {"Pid_view",":/images/setupmenu_system.png", "查看PID参数", UIHandler::PageId::Page_Setup_PID,true},
    {"Version",":/images/setupmenu_about.png", "关于", UIHandler::PageId::Page_Setup_Version,true}
};

SetupMenu::SetupMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupMenu)
{
    ui->setupUi(this);    

    connect(ui->listWidget,SIGNAL(clicked(QModelIndex)),this,SLOT(clickItem(QModelIndex)));
    ui->listWidget->setGeometry(0,0,ExGlobal::contentWidth,ExGlobal::contentHeight);
    //ui->listWidget->verticalScrollBar()->setStyleSheet("QScrollBar{width:100px;}");
    ui->listWidget->setVerticalScrollMode(QListWidget::ScrollMode::ScrollPerPixel);
    QScroller::grabGesture(ui->listWidget,QScroller::LeftMouseButtonGesture);
}

SetupMenu::~SetupMenu()
{
    delete ui;
}

void SetupMenu::showEvent(QShowEvent *event){
    Q_UNUSED(event);
    LoadMenu();
}

void SetupMenu::clickItem(QModelIndex index)
{
    UIHandler::GoPage(item[index.row()].page);
}

void SetupMenu::LoadMenu()
{
    ui->listWidget->clear();
    qDebug()<<sizeof(item[0])<<sizeof(item);
    for (unsigned int i = 0; i < sizeof(item)/sizeof(item[0]); i++){
        if (!item[i].show)
            continue;
        QListWidgetItem *list = new QListWidgetItem;
        list->setSizeHint(QSize(ExGlobal::contentWidth,80));
        ui->listWidget->addItem(list);

        QWidget *w = new QWidget(ui->listWidget);

        QLabel *icon = new QLabel(w);
        icon->setGeometry(106,15,50,50);
        icon->setPixmap(QPixmap(item[i].icon));

        QLabel *a = new QLabel(w);
        a->setText(item[i].name);
        a->setGeometry(175,0,500,80);
        a->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        a->setStyleSheet("font-size: 35px");

        QLabel *row = new QLabel(w);
        row->setGeometry(ExGlobal::contentWidth-21-120,38,21,37);
        row->setPixmap(QPixmap(":/images/row1.png"));

        QLabel *info = new QLabel(w);
        info->setGeometry(ExGlobal::contentWidth-21-120-350,15,380,80);
        info->setAlignment(Qt::AlignVCenter|Qt::AlignRight);
        info->setText(getItemInfo(item[i].Eid));
        info->setStyleSheet("font-size: 35px;color: #636363");

        ui->listWidget->setItemWidget(list,w);
    }
}

QString SetupMenu::getItemInfo(QString eid)
{
    QString itemInfo = "";
    if (eid == "SystemName")
        itemInfo = ExGlobal::getPtr()->sysName();
    else if (eid == "Language")
        itemInfo = ExGlobal::sysLanguageName();
    else if (eid == "Version")
        itemInfo = ExGlobal::version();
    else if (eid == "Network")
        itemInfo = ExGlobal::getIP();
    else if (eid == "Wlan")
        itemInfo = ExGlobal::getWIFIIP();
    else if (eid == "LockTime"){
        if (!ExGlobal::getPtr()->lockscreenOpen())
            itemInfo = tr("关闭");
        else if (ExGlobal::getPtr()->lockTime() == 0)
            itemInfo = "1 "+tr("分钟");
        else if (ExGlobal::getPtr()->lockTime() == 1)
            itemInfo = "3 "+tr("分钟");
        else if (ExGlobal::getPtr()->lockTime() == 2)
            itemInfo = "5 "+tr("分钟");
        else if (ExGlobal::getPtr()->lockTime() == 3)
            itemInfo = "10 "+tr("分钟");
        else if (ExGlobal::getPtr()->lockTime() == 4)
            itemInfo = "15 "+tr("分钟");
        else
            itemInfo = "30 "+tr("分钟");
    }

    return itemInfo;
}
