#include "dataview.h"
#include "ui_dataview.h"
#include "module/exglobal.h"
#include "module/uihandler.h"

#include <QPainter>
DataView::DataView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataView)
{
    ui->setupUi(this);    
    ui->Data_View_btBack->setGeometry(ExGlobal::contentWidth-299-80,ExGlobal::contentHeight-106-40,299,106);
    ui->Data_View_btCheck->setGeometry(ui->Data_View_btBack->x(),ui->Data_View_btBack->y()-106-30,299,106);
    ui->Data_View_btBack->setText(tr("返回"));

    ui->Data_View_lbPanelName->setGeometry(500,159,500,114);
    ui->Data_View_lbPanelName->setAlignment(Qt::AlignLeft|Qt::AlignVCenter);
    ui->Data_View_lbBoxSerial->setGeometry(ExGlobal::contentWidth-1000,159,500,114);
    ui->Data_View_lbBoxSerial->setAlignment(Qt::AlignRight|Qt::AlignVCenter);

    ui->Data_View_btPrint->setGeometry(ExGlobal::contentWidth-200-80,159+15,90,94);
    ui->Data_View_lbReference->setGeometry(700,273,500,114);
    ui->Data_View_btRef->setGeometry(1200,273,200,114);
}

DataView::~DataView()
{
    delete ui;
}

void DataView::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter paint(this);
    paint.fillRect(0,159,ExGlobal::contentWidth,114,QColor(0xdd,0xdd,0xdd));
    paint.fillRect(0,273,ExGlobal::contentWidth,114,QColor(0xf5,0xf5,0xf5));
}

void DataView::showEvent(QShowEvent *event){
    Q_UNUSED(event);
    ui->Data_View_lbPanelName->setText(ExGlobal::pTestModel->getCurrTestPanelName()+"   "+ExGlobal::pTestModel->getCurrTestPanelCode());
    ui->Data_View_lbBoxSerial->setText(ExGlobal::pTestModel->getCurrTestSerial());
    ui->Data_View_btCheck->setText(tr("通过审核"));
    ui->Data_View_lbReference->setText(tr("测试有效"));
    ui->Data_View_btRef->setText(ExGlobal::getPosName(2));

    QStringList itemName = ExGlobal::getBoxItemName();

    for (int i = 0; i < itemName.size(); i++){
        QPushButton *bt = new QPushButton(this);
        int x = i%4;
        int y = i/4;
        connect(bt,&QPushButton::clicked,this,&DataView::on_Item_clicked);
        bt->setGeometry(70+x*370,437+y*100,340,80);
        bt->setStyleSheet("text-align:left");
        bt->setObjectName(itemName[i]);
        bt->setText(itemName[i]);
        bt->setIconSize(QSize(40,40));
        bt->setIcon(QIcon(ExGlobal::pTestResultModel->getItemImgByName(ExGlobal::pTestResultModel->getTestid(),itemName[i])));
        bt->show();
        list.append(bt);
    }
    qDebug()<<itemName<<list.size();
}

void DataView::hideEvent(QHideEvent *event){
    Q_UNUSED(event);
    foreach (QPushButton *btn, list)
        delete btn;
    list.clear();
}

void DataView::on_Data_View_btCheck_clicked()
{

}

void DataView::on_Item_clicked(){
    if (ExGlobal::getCaliParam("MachineMode")==1){
        ExGlobal::pTestResultModel->setCurrItemName(focusWidget()->objectName());
        UIHandler::GoPage(UIHandler::PageId::Page_Data_Line);
    }
}

void DataView::on_Data_View_btBack_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Data);
}
