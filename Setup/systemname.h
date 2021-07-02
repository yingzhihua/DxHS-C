#ifndef SYSTEMNAME_H
#define SYSTEMNAME_H

#include <QDialog>

namespace Ui {
class SystemName;
}

class SystemName : public QDialog
{
    Q_OBJECT

public:
    explicit SystemName(QWidget *parent = nullptr);
    ~SystemName();

protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::SystemName *ui;
};

#endif // SYSTEMNAME_H
