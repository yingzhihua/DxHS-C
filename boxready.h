#ifndef BOXREADY_H
#define BOXREADY_H
#include "module/sequence.h"
#include "module/exglobal.h"
#include "module/uihandler.h"

#include <QWidget>

namespace Ui {
class BoxReady;
}

class BoxReady : public QWidget
{
    Q_OBJECT

public:
    explicit BoxReady(QWidget *parent = nullptr);
    ~BoxReady();

protected:
    void showEvent(QShowEvent *event);

private slots:
    void on_Home_BoxReady_btPen_clicked();
    void on_Sample_Changed(QString res1, QString res2, QString res3);
    void on_Home_BoxReady_btStart_clicked();

    void on_Home_BoxReady_btCancelTest_clicked();

private:
    Ui::BoxReady *ui;
    void ChangedTips();
};

#endif // BOXREADY_H
