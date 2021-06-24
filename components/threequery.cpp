#include "threequery.h"
#include "ui_threequery.h"
#include <QStyleOption>
#include <QPainter>
#include "../module/exglobal.h"

static ThreeQuery *threeQuery = nullptr;

ThreeQuery::ThreeQuery(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ThreeQuery)
{
    ui->setupUi(this);
    int init_x = (ExGlobal::contentWidth-810)/2;
    int init_y = (ExGlobal::contentHeight-565)/2+150;
    setGeometry(init_x,init_y,810,565);
    ui->Com_ThreeQuery_lbTitle->setGeometry(0,20,810,60);
    ui->Com_ThreeQuery_lbTitle->setAlignment(Qt::AlignTop|Qt::AlignHCenter);
    ui->Com_ThreeQuery_lbL1->setGeometry(50,150,200,60);
    ui->Com_ThreeQuery_lbL1->setAlignment(Qt::AlignRight);
    ui->Com_ThreeQuery_leR1->setGeometry(300,150,400,60);
    ui->Com_ThreeQuery_lbL2->setGeometry(50,240,200,60);
    ui->Com_ThreeQuery_lbL2->setAlignment(Qt::AlignRight);
    ui->Com_ThreeQuery_leR2->setGeometry(300,240,400,60);
    ui->Com_ThreeQuery_lbL3->setGeometry(50,330,200,60);
    ui->Com_ThreeQuery_lbL3->setAlignment(Qt::AlignRight);
    ui->Com_ThreeQuery_leR3->setGeometry(300,330,400,60);
    ui->Com_ThreeQuery_btCancel->setGeometry(42,434,250,85);
    ui->Com_ThreeQuery_btCancel->setText(tr("取消"));
    ui->Com_ThreeQuery_btConfirm->setGeometry(518,434,250,85);
    ui->Com_ThreeQuery_btConfirm->setText(tr("确认"));
}

ThreeQuery::~ThreeQuery()
{
    delete ui;
}

ThreeQuery *ThreeQuery::getPtr(){
    if (threeQuery == nullptr)
        threeQuery = new ThreeQuery;
    return threeQuery;
}

void ThreeQuery::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
}

void ThreeQuery::display(QString title, QString l1, QString l2, QString l3, QString v1, QString v2, QString v3)
{
    if (threeQuery == nullptr)
        threeQuery = new ThreeQuery;
    threeQuery->ui->Com_ThreeQuery_lbTitle->setText(title);
    threeQuery->ui->Com_ThreeQuery_lbL1->setText(l1);
    threeQuery->ui->Com_ThreeQuery_lbL2->setText(l2);
    threeQuery->ui->Com_ThreeQuery_lbL3->setText(l3);
    threeQuery->ui->Com_ThreeQuery_leR1->setText(v1);
    threeQuery->ui->Com_ThreeQuery_leR2->setText(v2);
    threeQuery->ui->Com_ThreeQuery_leR3->setText(v3);
    threeQuery->show();
}

void ThreeQuery::on_Com_ThreeQuery_btConfirm_clicked()
{    
    emit queryAck(ui->Com_ThreeQuery_leR1->text(),ui->Com_ThreeQuery_leR2->text(),ui->Com_ThreeQuery_leR3->text());
    threeQuery->close();
}

void ThreeQuery::on_Com_ThreeQuery_btCancel_clicked()
{
    threeQuery->close();
}
