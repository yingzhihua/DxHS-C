#include "errpage.h"
#include "ui_errpage.h"
#include "module/exglobal.h"

ErrPage::ErrPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrPage)
{
    ui->setupUi(this);
    ui->Main_ErrPage_btClose->setGeometry((ExGlobal::contentWidth-300)/2,(ExGlobal::contentHeight-107)/2-60,300,107);
    ui->Main_ErrPage_btClose->setText(tr("关机"));
    ui->Main_ErrPage_lbDep->setGeometry((ExGlobal::contentWidth-500)/2,ui->Main_ErrPage_btClose->y()+137,500,100);
    ui->Main_ErrPage_lbDep->setAlignment(Qt::AlignCenter);
    ui->Main_ErrPage_lb1->setGeometry((ExGlobal::contentWidth-500)/2,ui->Main_ErrPage_lbDep->y()+120,500,50);
    ui->Main_ErrPage_lb1->setAlignment(Qt::AlignCenter);
    ui->Main_ErrPage_lb1->setText(tr("请联系您的供应商"));
}

ErrPage::~ErrPage()
{
    delete ui;
}

void ErrPage::setError(QString error)
{
    ui->Main_ErrPage_lbDep->setText(error);
    qDebug()<<error;
}
