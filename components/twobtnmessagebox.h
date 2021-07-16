#ifndef TWOBTNMESSAGEBOX_H
#define TWOBTNMESSAGEBOX_H

#include <QWidget>

namespace Ui {
class TwoBtnMessageBox;
}

class TwoBtnMessageBox : public QWidget
{
    Q_OBJECT

public:
    explicit TwoBtnMessageBox(QWidget *parent = nullptr);
    ~TwoBtnMessageBox();
    static TwoBtnMessageBox *getPtr();
    static void display();

private slots:
    void on_Com_TwoBtnMessageBox_bt1_clicked();

    void on_Com_TwoBtnMessageBox_bt2_clicked();

private:
    Ui::TwoBtnMessageBox *ui;

signals:
    void MessageAck(int ack);
};

#endif // TWOBTNMESSAGEBOX_H
