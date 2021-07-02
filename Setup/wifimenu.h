#ifndef WIFIMENU_H
#define WIFIMENU_H

#include <QDialog>

namespace Ui {
class WifiMenu;
}

class WifiMenu : public QDialog
{
    Q_OBJECT

protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

public:
    explicit WifiMenu(QWidget *parent = nullptr);
    ~WifiMenu();

    void refreshWifi();
    void listview_refresh();
    void listview_click();
    void wifiShow();

    QString current_SSID;

private slots:
    void on_pushButton_3_clicked();

    void on_pButton_return_clicked();

    void on_pButton_refresh_clicked();

    void on_pButton_brock_clicked();

    void on_tableWidget_wifi_cellClicked(int row, int column);

    void Connect_Wifi(QString res1);
    void Connect_Finish(int val);
    void disconnect_wifi();

private:
    Ui::WifiMenu *ui;


};

#endif // WIFIMENU_H
