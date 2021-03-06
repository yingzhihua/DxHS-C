#include "usbfile.h"
#include "ui_usbfile.h"
#include "components/twobutton.h"

#include "../module/uihandler.h"
#include "../module/sequence.h"
#include "../module/exglobal.h"
#include "../module/dao/usbmodel.h"

#include <QLabel>
#include <QCheckBox>



UsbFile::UsbFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UsbFile)
{
    ui->setupUi(this);


    ui->pushButton->setGeometry(100,740,300,115);
    ui->pushButton_2->setGeometry(470,740,300,115);
    ui->pushButton_3->setGeometry(840,740,300,115);
    ui->pushButton_4->setGeometry(1210,740,300,115);
    ui->pushButton_5->setGeometry(1580,740,300,115);

  //  ui->gridLayoutWidget->setGeometry(10,50,1900,900);

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

  boxList.clear();

  for(int ii=0;ii<count;ii++)
    {
      CheckboxFile[ii]=new QCheckBox(this);
      LabelFile[ii] = new QLabel(this);

      boxList.append(CheckboxFile[ii]);

      ExGlobal::pUsbModel->setCurrIndex(ii);

      boxList[ii]->setFont(font_1);
      boxList[ii]->setText(ExGlobal::pUsbModel->getCurName());
      boxList[ii]->setChecked(ExGlobal::pUsbModel->getCurSelect());
      boxList[ii]->setIconSize(QSize(40,40));
      boxList[ii]->setStyleSheet("QCheckBox::indicator{width:35px;height:35px;bord-radius:7px}");


      LabelFile[ii]->setPixmap(ExGlobal::pUsbModel->getCurDir()?pixmap_dir:pixmap_file);
      qDebug()<<"count="<<count;
      qDebug()<<"ii=="<<ii;

      ui->gridLayout->addWidget(LabelFile[ii],   ii/4,(ii%4)*6,1,1);
      ui->gridLayout->addWidget(boxList[ii], ii/4,(ii%4)*6+1,1,5);

  }


 // usbModel.switchSelect(index)
}

void UsbFile::showEvent(QShowEvent *event){
    Q_UNUSED(event);
   show_file();
   connect(ExGlobal::pUsbModel,&UsbModel::copyFinish,this,&UsbFile::copyFinish);
}

void UsbFile::hideEvent(QHideEvent *event){
    Q_UNUSED(event);

}

void UsbFile::deleteGridItem(QLayout * layout)
{
    QLayoutItem *item;//*child,*item;

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

  //  TwoButton::display_one_bt(tr("??????"),tr("???????????????"),tr("??????"));
    Sequence::getPtr()->updateFooter(false,false,false);
    UIHandler::UpdateState(UIHandler::StateId::State_Loading_Open);
    ExGlobal::pUsbModel->copyDir();
}

void UsbFile::on_pushButton_3_clicked()
{
    TwoButton::display_one_bt(tr("??????"),ExGlobal::pUsbModel->updateSoft(),tr("??????"));
}

void UsbFile::on_pushButton_4_clicked()
{
    ExGlobal::pUsbModel->restart();
}

void UsbFile::on_pushButton_5_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Setup);
}

void UsbFile::copyFinish(int val)
{
    UIHandler::UpdateState(UIHandler::StateId::State_Loading_Close);
    if (val==0){
     TwoButton::display_one_bt(tr("??????"),tr("???????????????"),tr("??????"));
    }
    else if (val==1){
      TwoButton::display_one_bt(tr("??????"),tr("????????????U??????"),tr("??????"));
    }
    else if (val==2){
      TwoButton::display_one_bt(tr("??????"),tr("U???????????????????????????FAT32??????"),tr("??????"));
    }

    Sequence::getPtr()->updateFooter(true,true,true);
}
