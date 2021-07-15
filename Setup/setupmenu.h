#ifndef SETUPMENU_H
#define SETUPMENU_H

#include <QDialog>
#include <QStandardItemModel>

namespace Ui {
class SetupMenu;
}

class SetupMenu : public QDialog
{
    Q_OBJECT

public:
    explicit SetupMenu(QWidget *parent = nullptr);
    ~SetupMenu();

protected:
    void showEvent(QShowEvent *event);

private:
    Ui::SetupMenu *ui;
    void LoadMenu();
    QString getItemInfo(QString eid);

private slots:
    void clickItem(QModelIndex index);
};

#endif // SETUPMENU_H
