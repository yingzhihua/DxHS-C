#ifndef DATALINE_H
#define DATALINE_H

#include <QDialog>
//#include <QtCharts>
#include <QtCharts/QChart>
#include <QValueAxis>
#include <QCategoryAxis>
#include <QChartView>
#include <QLabel>
namespace Ui {
class DataLine;
}

QT_CHARTS_USE_NAMESPACE

class DataLine : public QDialog
{
    Q_OBJECT

public:
    explicit DataLine(QWidget *parent = nullptr);
    ~DataLine();

protected:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);

private slots:
    void on_btBack_clicked();

private:
    Ui::DataLine *ui;    
    QChart *chart;
    QChartView *chartView;
    QValueAxis *axisX;
    QCategoryAxis *axisY;

    QList<QLabel*> list;
};

#endif // DATALINE_H
