#include "languageset.h"
#include "ui_languageset.h"

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
