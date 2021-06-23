#include "boxready.h"
#include "ui_boxready.h"
#include <QListView>
BoxReady::BoxReady(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BoxReady)
{
    ui->setupUi(this);
    ui->Home_BoxReady_btStart->setGeometry(850,287,219,224);
    ui->Home_BoxReady_btPen->setGeometry(1621,606,75,70);
    ui->Home_BoxReady_btCancelTest->setGeometry(ExGlobal::contentWidth-299-50,ExGlobal::contentHeight-106-50,299,106);
    ui->Home_BoxReady_lbBK1->setGeometry(358,224,368,457);
    ui->Home_BoxReady_lbBK2->setGeometry(1195,224,368,457);
    ui->Home_BoxReady_lbTestName->setGeometry(368,244,348,140);
    ui->Home_BoxReady_lbTestSerial->setGeometry(368,389,348,140);
    ui->Home_BoxReady_cbTestType->setGeometry(390,570,300,60);
    ui->Home_BoxReady_cbTestType->setView(new QListView);
    ui->Home_BoxReady_lbSampleCode->setGeometry(2095,244,348,140);
    ui->Home_BoxReady_lbSampleInfo->setGeometry(2095,244,348,140);
    ui->Home_BoxReady_lbSampleRemark->setGeometry(2095,244,348,140);
    ui->Home_BoxReady_lbTestName->setAlignment(Qt::AlignCenter);
    ui->Home_BoxReady_lbTestSerial->setAlignment(Qt::AlignCenter);
    ui->Home_BoxReady_lbSampleCode->setAlignment(Qt::AlignCenter);
    ui->Home_BoxReady_lbSampleInfo->setAlignment(Qt::AlignCenter);
    ui->Home_BoxReady_lbSampleRemark->setAlignment(Qt::AlignCenter);

}

BoxReady::~BoxReady()
{
    delete ui;
}

void BoxReady::showEvent(QShowEvent *event){
    Q_UNUSED(event);
    ui->Home_BoxReady_cbTestType->clear();
    ui->Home_BoxReady_cbTestType->addItems(ExGlobal::getSampleTypeArr());
}

void BoxReady::on_Home_BoxReady_Start_clicked()
{
}
