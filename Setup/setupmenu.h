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
    void Init();

private:
    Ui::SetupMenu *ui;
    QStandardItemModel *model;

private slots:
    void clickItem(QModelIndex index);
};

#endif // SETUPMENU_H
