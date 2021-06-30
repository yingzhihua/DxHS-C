#include "twobutton.h"
#include "ui_twobutton.h"
#include "../module/exglobal.h"
#include <QStyleOption>
#include <QPainter>

static TwoButton *twobutton = nullptr;
static TwoButton *twobutton_no_edit = nullptr;

TwoButton::TwoButton(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TwoButton)
{
    ui->setupUi(this);

    int init_x = (ExGlobal::contentWidth-810)/2;
    int init_y = (ExGlobal::contentHeight-565)/2+150;
    setGeometry(init_x,init_y,810,565);
    ui->label_twobt_title->setGeometry(0,20,810,60);
    ui->label_twobt_title->setAlignment(Qt::AlignTop|Qt::AlignHCenter);
    ui->label_twobt_lb1->setGeometry(300,150,300,60);
    //ui->label_twobt_lb1->setAlignment(Qt::AlignRight);
    //ui->label_twobt_lb2->setGeometry(300,150,400,60);
    ui->label_twobt_lb2->setGeometry(150,240,200,60);
    ui->lEdit_twobt_edit1->setGeometry(300,240,400,60);
    ui->pButton_twobt_return->setGeometry(42,434,250,85);
    ui->pButton_twobt_return->setText(tr("连接"));
    ui->pButton_twobt_ok->setGeometry(518,434,250,85);
    ui->pButton_twobt_ok->setText(tr("取消"));
    ui->pButton_twobt_ok->setStyleSheet("QPushButton{border:3px black;background-color:rgb(200,200,200)}");
    ui->pButton_twobt_return->setStyleSheet("QPushButton{border:3px black;background-color:rgb(200,200,200)}");
}

TwoButton::~TwoButton()
{
    delete ui;
}

TwoButton *TwoButton::getPtr(){
    if (twobutton == nullptr)
        twobutton = new TwoButton();
    return twobutton;
}

TwoButton *TwoButton::getPtr_no_edit(){
    if (twobutton_no_edit == nullptr)
        twobutton_no_edit = new TwoButton();
    return twobutton_no_edit;
}

void TwoButton::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget,&opt,&p,this);
}


void TwoButton::display_two_bt(QString title, QString l1, QString l2, QString b1, QString b2,QString v1 )
{
    if (twobutton == nullptr)
        twobutton = new TwoButton;
     qDebug()<<"title:"<<title<<"+l1:"<<l1<<"+l2:"<<l2<<"+b1:"<<b1<<"+b2:"<<b2<<"+v1:"<<v1;
    twobutton->ui->label_twobt_title->setText(title);
    twobutton->ui->pButton_twobt_return->setText(b1);
    twobutton->ui->pButton_twobt_ok->setText(b2);
    twobutton->ui->label_twobt_lb1->setText(l1);
    twobutton->ui->label_twobt_lb2->setText(l2);
    twobutton->ui->lEdit_twobt_edit1->setText(v1);
    if (l2=="")
    {
      twobutton->ui->lEdit_twobt_edit1->hide();
      twobutton->ui->label_twobt_lb2->hide();
      twobutton->ui->label_twobt_lb1->setGeometry(270,220,400,60);
    }
    twobutton->show();

}

void TwoButton::display_two_bt_noedit(QString title, QString l1,QString b1, QString b2)
{
    if (twobutton_no_edit == nullptr)
        twobutton_no_edit = new TwoButton;
     qDebug()<<"title:"<<title<<"+l1:"<<l1<<"+b1:"<<b1<<"+b2:"<<b2;
    twobutton_no_edit->ui->label_twobt_title->setText(title);
    twobutton_no_edit->ui->pButton_twobt_return->setText(b1);
    twobutton_no_edit->ui->pButton_twobt_ok->setText(b2);
    twobutton_no_edit->ui->label_twobt_lb1->setText(l1);
    twobutton_no_edit->ui->label_twobt_lb2->setText("");
    twobutton_no_edit->ui->lEdit_twobt_edit1->setText("");
    twobutton_no_edit->ui->lEdit_twobt_edit1->hide();
    twobutton_no_edit->ui->label_twobt_lb2->hide();
    twobutton_no_edit->ui->label_twobt_lb1->setGeometry(270,220,400,60);

    twobutton_no_edit->show();

}

void TwoButton::on_pButton_twobt_ok_clicked()
{
     if (twobutton != nullptr)
       twobutton->close();
     if (twobutton_no_edit != nullptr)
       twobutton_no_edit->close();

}

void TwoButton::on_pButton_twobt_return_clicked()
{
    qDebug()<<"lEdit_twobt_edit1:"<<ui->lEdit_twobt_edit1->text();

    if (twobutton != nullptr)
    {
      twobutton->close();
      emit queryAck_twobt(ui->lEdit_twobt_edit1->text());
    }
    if (twobutton_no_edit != nullptr)
    {
      twobutton_no_edit->close();
      emit queryAck_twobt_no_edit();
    }

}
