#ifndef DATAMENU_H
#define DATAMENU_H

#include <QDialog>

namespace Ui {
class DataMenu;
}

class DataMenu : public QDialog
{
    Q_OBJECT

public:
    explicit DataMenu(QWidget *parent = nullptr);
    ~DataMenu();

protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private slots:
    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::DataMenu *ui;
};

#endif // DATAMENU_H
