#ifndef TIMESET_H
#define TIMESET_H

#include <QDialog>

namespace Ui {
class TimeSet;
}

class TimeSet : public QDialog
{
    Q_OBJECT

protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

public:
    explicit TimeSet(QWidget *parent = nullptr);
    ~TimeSet();

private slots:
    void on_calendarWidget_clicked(const QDate &date);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::TimeSet *ui;
};

#endif // TIMESET_H
