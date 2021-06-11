#include "setupmenu.h"
#include "ui_setupmenu.h"

#include <QDebug>
#include <QListWidgetItem>
#include <QScrollBar>

#include "../module/exglobal.h"
#include "../module/uihandler.h"

struct SetupMenuItem
{
    QString icon;
    QString name;
    UIHandler::PageId page;
};

const static SetupMenuItem item[] = {
    {":/images/setupmenu_system.png", "系统设置", UIHandler::PageId::Page_Setup_SystemName},
    {":/images/setupmenu_system.png", "锁屏时间", UIHandler::PageId::Page_Setup_LockScreen},
    {":/images/setupmenu_system.png", "背光设置", UIHandler::PageId::Page_Setup_Backlight},
    {":/images/setupmenu_system.png", "语言", UIHandler::PageId::Page_Setup_SystemName},
    {":/images/setupmenu_system.png", "修改密码", UIHandler::PageId::Page_Setup_SystemName},
    {":/images/setupmenu_system.png", "无线网络", UIHandler::PageId::Page_Setup_SystemName},
    {":/images/setupmenu_system.png", "有线网络", UIHandler::PageId::Page_Setup_SystemName},
    {":/images/setupmenu_system.png", "时间设置", UIHandler::PageId::Page_Setup_SystemName},
    {":/images/setupmenu_system.png", "用户帐号", UIHandler::PageId::Page_Setup_SystemName},
    {":/images/setupmenu_system.png", "机械校准", UIHandler::PageId::Page_Setup_SystemName},
    {":/images/setupmenu_system.png", "摄像头参数设置", UIHandler::PageId::Page_Setup_SystemName},
    {":/images/setupmenu_system.png", "测试设置", UIHandler::PageId::Page_Setup_SystemName},
    {":/images/setupmenu_system.png", "循环测试", UIHandler::PageId::Page_Setup_SystemName},
    {":/images/setupmenu_system.png", "系统参数", UIHandler::PageId::Page_Setup_SystemName},
    {":/images/setupmenu_system.png", "工程参数", UIHandler::PageId::Page_Setup_SystemName},
    {":/images/setupmenu_system.png", "数据维护", UIHandler::PageId::Page_Setup_SystemName},
    {":/images/setupmenu_system.png", "荧光强度", UIHandler::PageId::Page_Setup_SystemName},
    {":/images/setupmenu_system.png", "流体参数", UIHandler::PageId::Page_Setup_SystemName},
    {":/images/setupmenu_system.png", "查看Log信息", UIHandler::PageId::Page_Setup_SystemName},
    {":/images/setupmenu_system.png", "查看PID参数", UIHandler::PageId::Page_Setup_SystemName},
    {":/images/setupmenu_system.png", "关于", UIHandler::PageId::Page_Setup_SystemName}
};

SetupMenu::SetupMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetupMenu)
{
    ui->setupUi(this);
    model = new QStandardItemModel(this);

    connect(ui->listWidget,SIGNAL(clicked(QModelIndex)),this,SLOT(clickItem(QModelIndex)));
    ui->listWidget->setGeometry(0,0,ExGlobal::contentWidth,ExGlobal::contentHeight);
    ui->listWidget->verticalScrollBar()->setStyleSheet("QScrollBar{width:100px;}");

    qDebug()<<sizeof(item[0])<<sizeof(item);
    for (unsigned int i = 0; i < sizeof(item)/sizeof(item[0]); i++){
        QListWidgetItem *list = new QListWidgetItem;
        list->setSizeHint(QSize(ExGlobal::contentWidth,80));
        list->setIcon(QIcon(item[i].icon));
        list->setText(item[i].name);
        ui->listWidget->addItem(list);
    }
}

SetupMenu::~SetupMenu()
{
    delete ui;
}

void SetupMenu::Init()
{
    qDebug()<<"Setup Menu Init"<<this->width();
    model->clear();
    for (int i = 0; i < 100; i++)
        model->appendRow(new QStandardItem(QString("item %1").arg(i)));
}

void SetupMenu::clickItem(QModelIndex index)
{
    UIHandler::GoPage(item[index.row()].page);
}
