#ifndef IDLE_H
#define IDLE_H

#include <QDialog>

#include "module/sequence.h"

namespace Ui {
class Idle;
}

class Idle : public QDialog
{
    Q_OBJECT

public:
    explicit Idle(QWidget *parent = nullptr);
    ~Idle();

protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private slots:
    void on_Home_Idle_btOpenDoor_clicked();

private:
    Ui::Idle *ui;    
};

#endif // IDLE_H
