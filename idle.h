#ifndef IDLE_H
#define IDLE_H

#include <QDialog>

#include "module/sequence.h"
#include "module/exglobal.h"
#include "module/uihandler.h"
#include "components/threequery.h"
namespace Ui {
class Idle;
}

class Idle : public QDialog
{
    Q_OBJECT

public:
    explicit Idle(QWidget *parent = nullptr);
    ~Idle();
    virtual void timerEvent(QTimerEvent *e);

protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private slots:
    void on_Home_Idle_btOpenDoor_clicked();

    void on_Home_Idle_btLogout_clicked();

    void on_Home_Idle_btQuit_clicked();

    void StateUpdate();
    void sequenceFinish(Sequence::SequenceResult result);

private:
    Ui::Idle *ui;        
};

#endif // IDLE_H
