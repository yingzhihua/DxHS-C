#include "usbfile.h"
#include "ui_usbfile.h"

#include "../module/uihandler.h"
#include "../module/sequence.h"
#include "../module/exglobal.h"
#include <QLabel>
#include <QCheckBox>



UsbFile::UsbFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsbFile)
{
    ui->setupUi(this);
}

UsbFile::~UsbFile()
{
    delete ui;
}

void UsbFile::Checkbox_clicked(int index)
{

  ExGlobal::pUsbModel->switchSelect(index);
 qDebug()<<"Checkbox_clicked----index=="<<index;

}

void UsbFile::show_file(void)
{
  int count;//,row=0,col=0;
  QPixmap pixmap_dir(":/images/dir.png");
  QPixmap pixmap_file(":/images/file.png");
  QFont font_1=ui->pushButton->font();

  count=ExGlobal::pUsbModel->getCount();
  qDebug()<<"UsbFile::show_file ==count=="<<count;
  if (count==0) return ;
  QCheckBox * CheckboxFile[count];
  QLabel * LabelFile[count];
 // QLabel * LabelNull= new QLabel(this);

  boxList.clear();
 // LabelNull->setText("");

  for(int ii=0;ii<count;ii++)
    {
    // QCheckBox *cb = new QCheckBox(this);
     // boxList.append(cb);

      CheckboxFile[ii]=new QCheckBox(this);
      LabelFile[ii] = new QLabel(this);

      boxList.append(CheckboxFile[ii]);

      //connect(CheckboxFile[ii],SIGNAL(clicked()),this,SLOT(Checkbox_clicked(int)));
     // connect(CheckboxFile[ii],&QCheckBox::stateChanged,this,&UsbFile::Checkbox_clicked);
     // connect(CheckboxFile[ii],&QCheckBox::click,this,&UsbFile::Checkbox_clicked);

      ExGlobal::pUsbModel->setCurrIndex(ii);

    //  CheckboxFile[ii]->setFont(font_1);
    // CheckboxFile[ii]->setText(ExGlobal::pUsbModel->getCurName());
     // CheckboxFile[ii]->setChecked(ExGlobal::pUsbModel->getCurSelect());

      boxList[ii]->setFont(font_1);
      boxList[ii]->setText(ExGlobal::pUsbModel->getCurName());
      boxList[ii]->setChecked(ExGlobal::pUsbModel->getCurSelect());

      LabelFile[ii]->setPixmap(ExGlobal::pUsbModel->getCurDir()?pixmap_dir:pixmap_file);
     // LabelFile[ii]->setScaledContents(true);
     // LabelFile[ii]->setMaximumWidth(1);

    //  qDebug()<<"rowCount="<<ui->gridLayout->rowCount();
    // qDebug()<<"columnCount="<<ui->gridLayout->columnCount();
    // qDebug()<<"0columnMinimumWidth="<<ui->gridLayout->columnMinimumWidth(0);
     // qDebug()<<"1columnMinimumWidth="<<ui->gridLayout->columnMinimumWidth(1);
       qDebug()<<"count="<<count;
       qDebug()<<"ii=="<<ii;


      ui->gridLayout->addWidget(LabelFile[ii],   ii/4,(ii%4)*6,1,1);
     // ui->gridLayout->addWidget(CheckboxFile[ii], ii/4,(ii%4)*6+1,1,5);
      ui->gridLayout->addWidget(boxList[ii], ii/4,(ii%4)*6+1,1,5);


    // ui->gridLayout->addWidget(LabelNull,   ii/4,(ii%4)*6,1,6);

  }


 // usbModel.switchSelect(index)
}

void UsbFile::showEvent(QShowEvent *event){
    Q_UNUSED(event);
   show_file();

  /*// QFont font_1 =new QFont(); // QFont(this);
  int kk=60;
   // font_1->setPointSize(30);
   QFont font_1=ui->pushButton->font();

  QCheckBox * checkbox_1 =new QCheckBox(this);
  QCheckBox * checkbox_2 =new QCheckBox(this);

  QLabel * m_label1= new QLabel(this);
  QLabel * m_label2= new QLabel(this);

  QLabel * m_labeld[kk] ;

  QPixmap pixmap_dir(":/images/dir.png");
  QPixmap pixmap_file(":/images/file.png");

  for (int iii=0;iii<50;iii++)
  {
      m_labeld[iii]=new QLabel(this);
}
  checkbox_1->setFont(font_1);
  checkbox_1->setText("DxHSS");
  checkbox_1->setChecked(true);

 //m_label1->setFixedSize(40,40);
   m_label1->setPixmap(pixmap_dir);
  m_label1->setScaledContents(true);

   ui->gridLayout->addWidget(ui->label,   3,0,1,1);

    ui->label->setGeometry(20,20,30,40);
    ui->checkBox->setGeometry(60,20,300,40);
    ui->label_2->setGeometry(390,20,30,40);

    */

}

void UsbFile::hideEvent(QHideEvent *event){
    Q_UNUSED(event);

}

void UsbFile::deleteGridItem(QLayout * layout)
{
    QLayoutItem *item;//*child,*item;
/*
    if (layout==nullptr)
        return;
    while ((child =layout->takeAt(0))!=nullptr)
    {

        if (child->widget())
        {
            child->widget()->setParent(nullptr);
            delete child->widget();

        }else if(child->layout())
        {
              deleteGridItem(child->layout());
        }
        delete child;
        child=nullptr;
    }
    delete layout;
    layout=nullptr;
    */

    while((item=ui->gridLayout->takeAt(0))!=0)
    {
        if(item->widget())
        {
            ui->gridLayout->removeWidget(item->widget());
            item->widget()->setParent(0);
            delete item->widget();
         }
    delete item;
    }
}

void UsbFile::on_pushButton_clicked()
{
     int count ;
     count=ExGlobal::pUsbModel->getCount();
     for(int ii=0;ii<count;ii++)
     {
        if (boxList[ii]->checkState())
           ExGlobal::pUsbModel->switchSelect(ii);

     }
    ExGlobal::pUsbModel->deleteDir();
    deleteGridItem(ui->gridLayout);
  //  ui->gridLayout->setParent(NULL);
    show_file();
}

void UsbFile::on_pushButton_2_clicked()
{
    int count ;
    count=ExGlobal::pUsbModel->getCount();
    for(int ii=0;ii<count;ii++)
    {
       if (boxList[ii]->checkState())
          ExGlobal::pUsbModel->switchSelect(ii);
    }
    ExGlobal::pUsbModel->copyDir();
}

void UsbFile::on_pushButton_3_clicked()
{
    ExGlobal::pUsbModel->updateSoft();
}

void UsbFile::on_pushButton_4_clicked()
{
    ExGlobal::pUsbModel->restart();
}

void UsbFile::on_pushButton_5_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}
