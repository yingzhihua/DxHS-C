#ifndef DATAVIEW_H
#define DATAVIEW_H

#include <QDialog>

namespace Ui {
class DataView;
}

class DataView : public QDialog
{
    Q_OBJECT

public:
    explicit DataView(QWidget *parent = nullptr);
    ~DataView();

protected:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private slots:
    void on_Data_View_btCheck_clicked();
    void on_Item_clicked();

    void on_Data_View_btBack_clicked();

private:
    Ui::DataView *ui;
    QList<QPushButton*> list;
};

#endif // DATAVIEW_H
