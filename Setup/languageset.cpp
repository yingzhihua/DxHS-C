#include "languageset.h"
#include "ui_languageset.h"

#include "../module/exglobal.h"
#include "../module/uihandler.h"

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
}

LanguageSet::~LanguageSet()
{
    delete ui;
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
