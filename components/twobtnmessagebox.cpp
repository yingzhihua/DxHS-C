#include "twobtnmessagebox.h"
#include "ui_twobtnmessagebox.h"

static TwoBtnMessageBox *messageBox = nullptr;
TwoBtnMessageBox::TwoBtnMessageBox(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TwoBtnMessageBox)
{
    ui->setupUi(this);
}

TwoBtnMessageBox::~TwoBtnMessageBox()
{
    delete ui;
}

void TwoBtnMessageBox::display()
{
    if (messageBox == nullptr)
        messageBox = new TwoBtnMessageBox;
    messageBox->show();
}

void TwoBtnMessageBox::on_Com_TwoBtnMessageBox_bt1_clicked()
{
    messageBox->close();
    emit MessageAck(1);
}

void TwoBtnMessageBox::on_Com_TwoBtnMessageBox_bt2_clicked()
{
    messageBox->close();
    emit MessageAck(2);
}
