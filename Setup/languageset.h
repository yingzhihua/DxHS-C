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
    void addLang();
    void clickItem(QModelIndex index);

protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private slots:
    void on_radioButton_clicked();

    void on_radioButton_2_clicked();

    void on_pushButton_clicked();

    void on_listWidget_clicked(const QModelIndex &index);

private:
    Ui::LanguageSet *ui;
};

#endif // LANGUAGESET_H
