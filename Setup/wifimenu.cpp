#include "wifimenu.h"
#include "ui_wifimenu.h"

#include "../module/uihandler.h"
#include "../module/sequence.h"
#include "../module/exglobal.h"
#include <QLabel>
#include <QListWidgetItem>
#include "components/twobutton.h"
#include "../module/dao/wifimodel.h"

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

void WifiMenu::on_pushButton_3_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}
void WifiMenu::listview_refresh()
{
    /*
    QFont font_1=ui->pButton_return->font();
    QIcon pixmap_wifi_isconnect(":/images/wifiselect.png");
    QIcon pixmap_wifi_signal0(":/images/sign0.png");
    //QIcon picon1(":/images/wifiselect.png");
    QTableWidgetItem *item3,*item4;
    QListWidgetItem  *item1,*item2;
    QStringList strs={"是否连接","wifi名称","信号强度"};
   // strs<<"是否连接"<<"wifi=名称"<<"信号强度"<<"";

  // ui->tableWidget_wifi->setHorizontalHeaderLabels(strs);



   ui->tableWidget_wifi->setRowCount(5);
    ui->tableWidget_wifi->setColumnCount(3);
     ui->tableWidget_wifi->setRowHeight(0,60);
  ui->tableWidget_wifi->setRowHeight(1,60);
   ui->tableWidget_wifi->setRowHeight(2,60);
     ui->tableWidget_wifi->setRowHeight(3,60);
       ui->tableWidget_wifi->setColumnWidth(0,200);
     ui->tableWidget_wifi->setColumnWidth(1,200);
     ui->tableWidget_wifi->setColumnWidth(2,200);
     ui->tableWidget_wifi->setColumnWidth(3,200);

     ui->tableWidget_wifi->setIconSize(QSize(40,40));
     ui->listWidget_wifi->setIconSize(QSize(40,40));


    item1=new QListWidgetItem();
    item2=new QListWidgetItem();

    item3=new QTableWidgetItem();
    item4=new QTableWidgetItem();

    item1->setIcon(pixmap_wifi_isconnect);
    item2->setIcon(pixmap_wifi_signal0);


    item1->setText("确定");
    item2->setText("返回");

    item1->setFont(font_1);
    item2->setFont(font_1);




    item3->setIcon(pixmap_wifi_isconnect);
    item4->setIcon(pixmap_wifi_signal0);

    item3->setText("确定");
    item4->setText("返回");

    item3->setFont(font_1);
    item4->setFont(font_1);

    ui->tableWidget_wifi->setItem(0,0,item3);
    ui->tableWidget_wifi->setItem(1,0,item4);

    ui->listWidget_wifi->addItem(item1);
    ui->listWidget_wifi->addItem(item2);


    ui->tableWidget_wifi->setSelectionBehavior(QAbstractItemView::SelectItems);
    ui->tableWidget_wifi->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_wifi->horizontalHeader()->setStretchLastSection(true);

    font_1.setPixelSize(50);
    font_1.setPointSize(40);
    ui->tableWidget_wifi->horizontalHeader()->setFont(font_1);        //QFont("song",40));
    ui->tableWidget_wifi->verticalHeader()->setHidden(true);
    //ui->tableWidget_wifi->verticalHeader()->setFont(font_1);
    ui->tableWidget_wifi->horizontalHeader()->setHighlightSections(false);
    ui->tableWidget_wifi->verticalHeader()->setHighlightSections(true);
  //  ui->tableWidget_wifi->horizontalHeader()->setFont();
    ui->tableWidget_wifi->setWindowTitle("可用wifi列表");

   ui->tableWidget_wifi->setHorizontalHeaderLabels(strs);


  //  ui->tableWidget_wifi->setItem(0,3,new QLabel(":/images/sign0.png"));

*/
}
void WifiMenu::wifiShow()
{
   QFont font_title=ui->pButton_refresh->font();
   int wifi_count=ExGlobal::pWifiModel->getCount();//>rowCount();
   QPixmap pixmap_wifi_isconnect(":/images/wifiselect.png");
   QPixmap pixmap_wifi_signal0(":/images/sign0.png");
   QPixmap pixmap_wifi_signal1(":/images/sign1.png");
   QPixmap pixmap_wifi_signal2(":/images/sign2.png");
   QPixmap pixmap_wifi_signal3(":/images/sign3.png");
   QTableWidgetItem *item_connect[wifi_count],*item_SSID[wifi_count],*item_signal[wifi_count];
   QStringList str_title={"是否连接","wifi名称","信号强度"};

   ui->tableWidget_wifi->clear();
   ui->tableWidget_wifi->setSelectionBehavior(QAbstractItemView::SelectItems);  //行选择
   ui->tableWidget_wifi->setEditTriggers(QAbstractItemView::NoEditTriggers);   //不能编辑
   ui->tableWidget_wifi->horizontalHeader()->setStretchLastSection(true);  //表头自适应
   ui->tableWidget_wifi->setFrameShape(QFrame::NoFrame); //无边框
  // ui->tableWidget_wifi->setShowGrid(false); //不显示网格

   ui->tableWidget_wifi->setIconSize(QSize(40,40));
   ui->tableWidget_wifi->setRowCount(wifi_count);
   ui->tableWidget_wifi->setColumnCount(3);

   ui->tableWidget_wifi->horizontalHeader()->setDefaultSectionSize(50); //设置缺省列宽度
   ui->tableWidget_wifi->verticalHeader()->setDefaultSectionSize(50); //设置缺省行高度
   ui->tableWidget_wifi->setColumnWidth(0,100);
   ui->tableWidget_wifi->setColumnWidth(1,600);
   ui->tableWidget_wifi->setColumnWidth(2,100);


   for(int ii=0;ii<wifi_count;ii++)
   {
       ExGlobal::pWifiModel->setCurrIndex(ii);
       ui->tableWidget_wifi->setRowHeight(ii,70);
       item_connect[ii]=new QTableWidgetItem();
       item_SSID[ii]=new QTableWidgetItem();
       item_signal[ii]=new QTableWidgetItem();
       if (ExGlobal::pWifiModel->getCurConnect())
           item_connect[ii]->setIcon(pixmap_wifi_isconnect);

       item_SSID[ii]->setText(ExGlobal::pWifiModel->getCurSSID());

       if (ExGlobal::pWifiModel->getCurSignal()>90)
         item_signal[ii]->setIcon(pixmap_wifi_signal3);
       else if (ExGlobal::pWifiModel->getCurSignal()>60)
         item_signal[ii]->setIcon(pixmap_wifi_signal2);
       else if (ExGlobal::pWifiModel->getCurSignal()>10)
         item_signal[ii]->setIcon(pixmap_wifi_signal1);
       else
         item_signal[ii]->setIcon(pixmap_wifi_signal1);

       ui->tableWidget_wifi->setItem(ii,0,item_connect[ii]);
       ui->tableWidget_wifi->setItem(ii,1,item_SSID[ii]);
       ui->tableWidget_wifi->setItem(ii,2,item_signal[ii]);
   }


   ui->tableWidget_wifi->horizontalHeader()->setFont(font_title);        //QFont("song",40));
   ui->tableWidget_wifi->verticalHeader()->setHidden(true);  //隐藏行标题
   ui->tableWidget_wifi->horizontalHeader()->setHighlightSections(false); //表头不高亮
   //ui->tableWidget_wifi->verticalHeader()->setHighlightSections(true);
   ui->tableWidget_wifi->setWindowTitle("可用wifi列表");
   ui->tableWidget_wifi->setHorizontalHeaderLabels(str_title);

}

void WifiMenu::listview_click()
{

}

void WifiMenu::refreshWifi()
{

}

void WifiMenu::showEvent(QShowEvent *event){
    Q_UNUSED(event);

  // refreshWifi();
  //  listview_refresh();
  wifiShow();
  connect(TwoButton::getPtr(),&TwoButton::queryAck_twobt,this,&WifiMenu::Connect_Wifi);
  connect(ExGlobal::pWifiModel,&WifiModel::finish,this,&WifiMenu::Connect_Finish);
  connect(TwoButton::getPtr_no_edit(),&TwoButton::queryAck_twobt_no_edit,this,&WifiMenu::disconnect_wifi);
}

void WifiMenu::hideEvent(QHideEvent *event){
    Q_UNUSED(event);
   TwoButton::getPtr()->disconnect(this);
   TwoButton::getPtr_no_edit()->disconnect(this);
}

void WifiMenu::on_pButton_return_clicked()
{
     UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}

void WifiMenu::on_pButton_refresh_clicked()
{
    UIHandler::UpdateState(UIHandler::StateId::State_Loading_Open);
    ExGlobal::pWifiModel->refresh();
    wifiShow();
    UIHandler::UpdateState(UIHandler::StateId::State_Loading_Close);
}

void WifiMenu::on_pButton_brock_clicked()
{
    TwoButton::display_two_bt_noedit(tr("断开wifi连接"),tr("是否断开wifi连接？"),
                        tr("确定"),tr("取消"));
}

void WifiMenu::disconnect_wifi()
{
     UIHandler::UpdateState(UIHandler::StateId::State_Loading_Open);
     ExGlobal::pWifiModel->disconnect();
     wifiShow();
     UIHandler::UpdateState(UIHandler::StateId::State_Loading_Close);

}


void WifiMenu::on_tableWidget_wifi_cellClicked(int row, int column)
{
    QAbstractItemModel *model=ui->tableWidget_wifi->model();
    QModelIndex index=model->index(row,1);
    QVariant data=model->data(index);
    current_SSID=data.toString();
    TwoButton::display_two_bt(tr("输入wifi密码"),data.toString(),tr("密码:"),
                        tr("连接"),tr("取消"),"flashdxx");
}

void WifiMenu::Connect_Wifi(QString res1)
{
  qDebug()<<"wifi_connect:"<<current_SSID<<"++++++++++++++++++++++++wifi_password:"<<res1;
  UIHandler::UpdateState(UIHandler::StateId::State_Loading_Open);
  ExGlobal::pWifiModel->connect(current_SSID,res1);
}


void WifiMenu::Connect_Finish(int val)
{
  qDebug()<<"wifi_connect:finish";
  UIHandler::UpdateState(UIHandler::StateId::State_Loading_Close);
  wifiShow();
}
