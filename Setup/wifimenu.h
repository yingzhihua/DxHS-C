#ifndef WIFIMENU_H
#define WIFIMENU_H

#include <QDialog>

namespace Ui {
class WifiMenu;
}

class WifiMenu : public QDialog
{
    Q_OBJECT

public:
    explicit WifiMenu(QWidget *parent = nullptr);
    ~WifiMenu();

private:
    Ui::WifiMenu *ui;
};

#endif // WIFIMENU_H
