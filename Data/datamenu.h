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

private:
    Ui::DataMenu *ui;
};

#endif // DATAMENU_H
