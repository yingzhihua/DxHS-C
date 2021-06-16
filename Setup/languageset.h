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

private slots:
    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

private:
    Ui::LanguageSet *ui;
};

#endif // LANGUAGESET_H
