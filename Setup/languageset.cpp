#include "languageset.h"
#include "ui_languageset.h"
#include "../module/exglobal.h"

LanguageSet::LanguageSet(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LanguageSet)
{
    ui->setupUi(this);
}

LanguageSet::~LanguageSet()
{
    delete ui;
}

void LanguageSet::on_radioButton_clicked()
{
     //ExGlobal::setSysLanguageCode(0) ;
     ExGlobal::getPtr()->setSysLanguageCode(0);
}

void LanguageSet::on_radioButton_2_clicked()
{
     ExGlobal::getPtr()->setSysLanguageCode(1);
}
