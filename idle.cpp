#include "idle.h"
#include "ui_idle.h"

Idle::Idle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Idle)
{
    ui->setupUi(this);
}

Idle::~Idle()
{
    delete ui;
}

void Idle::on_btOpenDoor_clicked()
{
    if (Sequence::getPtr()->readDoorState())
        Sequence::getPtr()->sequenceDo(Sequence::SequenceId::Sequence_CloseBox);
    else
        Sequence::getPtr()->sequenceDo(Sequence::SequenceId::Sequence_OpenBox);
}
