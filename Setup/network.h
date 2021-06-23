#ifndef NETWORK_H
#define NETWORK_H

#include <QDialog>

namespace Ui {
class NetWork;
}

class NetWork : public QDialog
{
    Q_OBJECT

public:
    explicit NetWork(QWidget *parent = nullptr);
    ~NetWork();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_ok_clicked();

    void on_pushButton_cencel_clicked();

private:
    Ui::NetWork *ui;
};

#endif // NETWORK_H
