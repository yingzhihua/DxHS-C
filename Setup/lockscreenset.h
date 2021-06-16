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

private slots:
    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_2_clicked();

private:
    Ui::LockScreenSet *ui;
};

#endif // LOCKSCREENSET_H
