#ifndef SETUPMENU_H
#define SETUPMENU_H

#include <QDialog>

namespace Ui {
class SetupMenu;
}

class SetupMenu : public QDialog
{
    Q_OBJECT

public:
    explicit SetupMenu(QWidget *parent = nullptr);
    ~SetupMenu();

private:
    Ui::SetupMenu *ui;
};

#endif // SETUPMENU_H
