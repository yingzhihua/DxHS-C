#ifndef LANGUAGESET_H
#define LANGUAGESET_H

#include <QDialog>

namespace Ui {
class LanguageSet;
}

class LanguageSet : public QDialog
{
    Q_OBJECT

public:
    explicit LanguageSet(QWidget *parent = nullptr);
    ~LanguageSet();

private:
    Ui::LanguageSet *ui;
};

#endif // LANGUAGESET_H
