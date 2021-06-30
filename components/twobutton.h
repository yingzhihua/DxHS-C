#ifndef TWOBUTTON_H
#define TWOBUTTON_H

#include <QDialog>

namespace Ui {
class TwoButton;
}

class TwoButton : public QDialog
{
    Q_OBJECT

public:
    explicit TwoButton(QWidget *parent = nullptr);
    ~TwoButton();
    static TwoButton *getPtr();
    static TwoButton *getPtr_no_edit();
    static void display_two_bt(QString title, QString l1, QString l2, QString b1, QString b2,QString v1 );
    static void display_two_bt_noedit(QString title, QString l1,QString b1, QString b2);
protected:
    void paintEvent(QPaintEvent *event);


private slots:
    void on_pButton_twobt_ok_clicked();

    void on_pButton_twobt_return_clicked();

private:
    Ui::TwoButton *ui;

signals:
    void queryAck_twobt(QString res1);
    void queryAck_twobt_no_edit();
};

#endif // TWOBUTTON_H
