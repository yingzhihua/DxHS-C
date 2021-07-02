#ifndef LOCKSCREENSET_H
#define LOCKSCREENSET_H

#include <QDialog>

namespace Ui {
class LockScreenSet;
}

class LockScreenSet : public QDialog
{
    Q_OBJECT

public:
    explicit LockScreenSet(QWidget *parent = nullptr);
    ~LockScreenSet();
protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_2_clicked();

    void on_pushButton_switch_clicked();

    void on_comboBox_time_currentIndexChanged(int index);

    void on_pushButton_return_clicked();

private:
    Ui::LockScreenSet *ui;
};

#endif // LOCKSCREENSET_H
