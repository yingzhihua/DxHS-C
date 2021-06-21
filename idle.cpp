#include "idle.h"
#include "ui_idle.h"
#include "module/exglobal.h"

Idle::Idle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Idle)
{
    ui->setupUi(this);
    ui->Home_Idle_btOpenDoor->setGeometry((ExGlobal::contentWidth-219)/2,(ExGlobal::contentHeight-224)/2,219,224);
    ui->Home_Idle_lbBoxState->setGeometry((ExGlobal::contentWidth-400)/2,ui->Home_Idle_btOpenDoor->y()+280,400,60);
    ui->Home_Idle_lbBoxState->setAlignment(Qt::AlignCenter);
    ui->Home_Idle_lbBoxState->setText(tr("未检测到试剂盒"));
    ui->Home_Idle_btQuit->setGeometry(ExGlobal::contentWidth-299-50,ExGlobal::contentHeight-106-50,299,106);
    ui->Home_Idle_btQuit->setText(tr("关机"));
    ui->Home_Idle_btLogout->setGeometry(ui->Home_Idle_btQuit->x(),ui->Home_Idle_btQuit->y()-106-30,299,106);
    ui->Home_Idle_btLogout->setText(tr("退出登录"));
}

Idle::~Idle()
{
    delete ui;
}

void Idle::showEvent(QShowEvent *event){
    Q_UNUSED(event);
}

void Idle::hideEvent(QHideEvent *event){
    Q_UNUSED(event);
}

void Idle::on_Home_Idle_btOpenDoor_clicked()
{
    if (Sequence::getPtr()->readDoorState())
        Sequence::getPtr()->sequenceDo(Sequence::SequenceId::Sequence_CloseBox);
    else
        Sequence::getPtr()->sequenceDo(Sequence::SequenceId::Sequence_OpenBox);
}
