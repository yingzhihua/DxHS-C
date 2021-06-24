#ifndef THREEQUERY_H
#define THREEQUERY_H

#include <QWidget>

namespace Ui {
class ThreeQuery;
}

class ThreeQuery : public QWidget
{
    Q_OBJECT

public:
    explicit ThreeQuery(QWidget *parent = nullptr);
    ~ThreeQuery();
    static ThreeQuery *getPtr();
    static void display(QString title, QString l1, QString l2, QString l3, QString v1 = "", QString v2 = "", QString v3 = "");

protected:
    void paintEvent(QPaintEvent *event);

private slots:
    void on_Com_ThreeQuery_btConfirm_clicked();

    void on_Com_ThreeQuery_btCancel_clicked();

private:
    Ui::ThreeQuery *ui;

signals:
    void queryAck(QString res1, QString res2, QString res3);
};

#endif // THREEQUERY_H
