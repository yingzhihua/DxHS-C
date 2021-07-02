#include "datamenu.h"
#include "ui_datamenu.h"
#include "module/exglobal.h"
#include "module/uihandler.h"

#include <QStringListModel>
#include <QScrollBar>
#include <QScroller>
#include <QStandardItemModel>
DataMenu::DataMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataMenu)
{
    ui->setupUi(this);
    ui->Data_Menu_lbHeader1->setGeometry(0,0,100,80);
    ui->Data_Menu_lbHeader1->setAlignment(Qt::AlignCenter);
    ui->Data_Menu_lbHeader2->setGeometry(100,0,200,80);
    ui->Data_Menu_lbHeader2->setAlignment(Qt::AlignCenter);
    ui->Data_Menu_lbHeader3->setGeometry(300,0,500,80);
    ui->Data_Menu_lbHeader3->setAlignment(Qt::AlignCenter);
    ui->Data_Menu_lbHeader4->setGeometry(800,0,300,80);
    ui->Data_Menu_lbHeader4->setAlignment(Qt::AlignCenter);
    ui->Data_Menu_lbHeader5->setGeometry(1100,0,200,80);
    ui->Data_Menu_lbHeader5->setAlignment(Qt::AlignCenter);
    ui->Data_Menu_lbHeader6->setGeometry(1300,0,200,80);
    ui->Data_Menu_lbHeader6->setAlignment(Qt::AlignCenter);
    ui->Data_Menu_lbHeader7->setGeometry(1500,0,400,80);
    ui->Data_Menu_lbHeader7->setAlignment(Qt::AlignCenter);

    ui->Data_Menu_lbHeader1->setText(tr("序号"));
    ui->Data_Menu_lbHeader2->setText(tr("是否异常"));
    ui->Data_Menu_lbHeader3->setText(tr("测试项目"));
    ui->Data_Menu_lbHeader4->setText(tr("样本号"));
    ui->Data_Menu_lbHeader5->setText(tr("操作员"));
    ui->Data_Menu_lbHeader6->setText(tr("审核员"));
    ui->Data_Menu_lbHeader7->setText(tr("测试时间"));

    ui->tableView->setGeometry(0,100,ExGlobal::contentWidth,ExGlobal::contentHeight-100);
    ui->tableView->setVerticalScrollMode(QTableView::ScrollMode::ScrollPerPixel);
    QScroller::grabGesture(ui->tableView,QScroller::LeftMouseButtonGesture);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->horizontalHeader()->hide();
    ui->tableView->verticalHeader()->setDefaultSectionSize(100);
    //ui->tableView->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{width:100px;}");
    ui->tableView->setModel(ExGlobal::pTestModel);
    ui->tableView->setAlternatingRowColors(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setShowGrid(false);
    ui->tableView->setColumnWidth(0,100);
    ui->tableView->setColumnWidth(1,200);
    ui->tableView->setColumnWidth(2,500);
    ui->tableView->setColumnWidth(3,300);
    ui->tableView->setColumnWidth(4,200);
    ui->tableView->setColumnWidth(5,200);
    ui->tableView->setColumnWidth(6,400);
}

DataMenu::~DataMenu()
{
    delete ui;
}

void DataMenu::showEvent(QShowEvent *event){
    Q_UNUSED(event);
}

void DataMenu::hideEvent(QHideEvent *event){
    Q_UNUSED(event);

}

void DataMenu::on_tableView_clicked(const QModelIndex &index)
{
    qDebug()<<"clicked"<<index.row();
    ExGlobal::pTestModel->setCurrTest(index.row());
    ExGlobal::getPtr()->setDataEntry(0);
    UIHandler::GoPage(UIHandler::PageId::Page_Data_View);
}
