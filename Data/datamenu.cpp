#include "datamenu.h"
#include "ui_datamenu.h"
#include "module/exglobal.h"
#include <QStringListModel>
#include <QScrollBar>
#include <QScroller>
DataMenu::DataMenu(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataMenu)
{
    ui->setupUi(this);
    ui->listView->setGeometry(0,0,ExGlobal::contentWidth,ExGlobal::contentHeight);
    ui->listView->setVerticalScrollMode(QListView::ScrollMode::ScrollPerPixel);
    QScroller::grabGesture(ui->listView,QScroller::LeftMouseButtonGesture);
    ui->listView->setModelColumn(5);
    ui->listView->setModel(ExGlobal::pTestModel);
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
