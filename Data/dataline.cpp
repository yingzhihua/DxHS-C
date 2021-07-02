#include "dataline.h"
#include "ui_dataline.h"
#include <QtCharts/QChartView>
#include <QLineSeries>
#include "module/exglobal.h"
#include "module/uihandler.h"
DataLine::DataLine(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DataLine)
{
    ui->setupUi(this);
    chart = new QChart;
    axisX = new QValueAxis;
    axisX->setRange(0,50);
    axisX->setTitleText("cycle");
    axisX->setTickCount(6);
    axisX->setMinorTickCount(9);
    axisX->setLabelFormat("%d");
    axisY = new QCategoryAxis;
    axisY->setRange(-10,50);
    axisY->setTitleText("brightness");
    axisY->setLabelsPosition(QCategoryAxis::AxisLabelsPositionOnValue);
    chart->addAxis(axisX,Qt::AlignBottom);
    chart->addAxis(axisY,Qt::AlignLeft);
    chartView = new QChartView(chart,this);

    chartView->setGeometry(600,100,1300,600);
    chart->setTitle("lengxing");
    chart->setTitleFont(QFont("黑体",30));

    ui->btBack->setGeometry(ExGlobal::contentWidth-299-80,ExGlobal::contentHeight-106-40,299,106);
    ui->btBack->setText(tr("返回"));
}

DataLine::~DataLine()
{
    delete ui;
}

void DataLine::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter paint(this);
    QList<int> items = ExGlobal::pTestResultModel->getCurrItemResultByName();
    paint.setFont(QFont("黑体",20));
    paint.drawText(130,200,tr("靶标"));
    paint.drawText(360,200,tr("Ct"));
    for (int i = 0; i < items.size(); i++){
        paint.drawText(130,250+i*50,ExGlobal::pTestResultModel->getSubNameByPos(items[i]/1000));
        if (items[i]%1000==0)
            paint.drawText(360,250+i*50,"ND");
        else
            paint.drawText(360,250+i*50,QString::number(items[i]%1000/10));
    }
}

void DataLine::showEvent(QShowEvent *event){
    Q_UNUSED(event);
    if (ExGlobal::isDebug())
        chartView->setGeometry(600,100,1300,600);
    chart->setTitle(ExGlobal::pTestResultModel->getCurrItemName());
    chart->removeAllSeries();
    int ct = ExGlobal::pTestResultModel->getCurrCT();
    axisY->append(QString::number(ct),ct);
    for (int i = 0; i < ExGlobal::pTestResultModel->rowCount(); i++){
        QString seriesName = QString("P%1").arg(ExGlobal::pTestResultModel->getField(i,"PosIndex").toInt());
        QLineSeries *line = nullptr;
        double value = ExGlobal::pTestResultModel->getField(i,"TestValue").toDouble();
        int cycle = ExGlobal::pTestResultModel->getField(i,"cycle").toInt();
        //int itemid = ExGlobal::pTestResultModel->getField(i,"Itemid").toInt();
        foreach (QAbstractSeries *series, chart->series()){
            if (series->name() == seriesName){
                line = static_cast<QLineSeries *>(series);
                break;
            }
        }
        if (line == nullptr)
        {
            line = new QLineSeries;
            line->setName(seriesName);

            chart->addSeries(line);
            line->attachAxis(axisX);
            line->attachAxis(axisY);
        }
        if (value > axisY->max() - 10)
            axisY->setMax(value+10);
        if (cycle > axisX->max())
            axisX->setMax(cycle);
        line->append(cycle,value);
    }

}

void DataLine::hideEvent(QHideEvent *event){
    Q_UNUSED(event);
    foreach (QString label, axisY->categoriesLabels())
        axisY->remove(label);
    foreach (QLabel *label, list)
        delete label;
    list.clear();
}

void DataLine::on_btBack_clicked()
{
    UIHandler::GoPage(UIHandler::PageId::Page_Data_View);
}
