#include "languageset.h"
#include "ui_languageset.h"

#include "../module/exglobal.h"
#include "../module/uihandler.h"
#include <QLabel>
#include <QScroller>

LanguageSet::LanguageSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LanguageSet)
{
    ui->setupUi(this);


    //QRadioButton::indi
    ui->radioButton->setGeometry(750,300,500,60);
    ui->radioButton_2->setGeometry(750,400,500,60);
    ui->pushButton->setGeometry(1580,740,300,115);

    ui->radioButton->setIconSize(QSize(40,40));
    ui->radioButton_2->setIconSize(QSize(40,40));
    ui->radioButton->setStyleSheet("QRadioButton::indicator{width:35px;height:35px;bord-radius:7px}"
                                  // "QRadioButton::indicator:checked{background-color:green;}"
                                   );
    ui->radioButton_2->setStyleSheet("QRadioButton::indicator{width:35px;height:35px;bord-radius:7px}"
                                  // "QRadioButton::indicator:checked{background-color:green;}"
                                   );
   if (ExGlobal::getPtr()->sysLanguageCode()==0)
       ui->radioButton->setChecked(true);
    else
       ui->radioButton_2->setChecked(true);

   ui->listWidget->setGeometry(0,0,ExGlobal::contentWidth,ExGlobal::contentHeight);
   ui->radioButton->setVisible(false);
    ui->radioButton_2->setVisible(false);
   ui->pushButton->setVisible(false);

   // connect(ui->listWidget,SIGNAL(clicked(QModelIndex)),this,SLOT(clickItem(QModelIndex)));
    //ui->listWidget->setVerticalScrollMode(QListWidget::ScrollMode::ScrollPerPixel);
   // QScroller::grabGesture(ui->listWidget,QScroller::LeftMouseButtonGesture);



}

LanguageSet::~LanguageSet()
{
    delete ui;
}

void LanguageSet::clickItem(QModelIndex index)
{
  qDebug()<<"item_index===="<<index<<"item_index.row()===="<<index.row();
  ExGlobal::getPtr()->setSysLanguageCode(index.row());
   addLang();
}
void LanguageSet::showEvent(QShowEvent *event){
    Q_UNUSED(event);
    addLang();
}

void LanguageSet::hideEvent(QHideEvent *event){
    Q_UNUSED(event);

}


void LanguageSet::addLang()
{
    ui->listWidget->clear();
    for (unsigned int i = 0; i < 2; i++){
        QListWidgetItem *list = new QListWidgetItem;
        list->setSizeHint(QSize(ExGlobal::contentWidth,80));
        ui->listWidget->addItem(list);

        QWidget *w = new QWidget(ui->listWidget);

        QLabel *a = new QLabel(w);
        if (i==0)
          a->setText("中文");
        else
          a->setText("English");

        a->setGeometry(160,20,200,49);
        a->setAlignment(Qt::AlignVCenter|Qt::AlignLeft);
        a->setStyleSheet("font-size: 40px");

        QLabel *row = new QLabel(w);
        row->setGeometry(ExGlobal::contentWidth-140,38,60,45);
        if (ExGlobal::getPtr()->sysLanguageCode()==0)
        {
            if (i==0)
           row->setPixmap(QPixmap(":/images/check.png"));
        }
        else
        {
            if (i==1)
           row->setPixmap(QPixmap(":/images/check.png"));
        }

        ui->listWidget->setItemWidget(list,w);
    }

}


void LanguageSet::on_radioButton_clicked()
{
     ExGlobal::getPtr()->setSysLanguageCode(0);

}

void LanguageSet::on_radioButton_2_clicked()
{
     ExGlobal::getPtr()->setSysLanguageCode(1);
}

void LanguageSet::on_pushButton_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}

void LanguageSet::on_listWidget_clicked(const QModelIndex &index)
{
    qDebug()<<"item_index===="<<index<<"item_index.row()===="<<index.row();
    ExGlobal::getPtr()->setSysLanguageCode(index.row());
   //  addLang();
     UIHandler::GoPage(UIHandler::PageId::Page_Setup);

}
