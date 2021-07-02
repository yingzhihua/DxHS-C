#include "logview.h"
#include "ui_logview.h"
#include "../module/dao/logmodel.h"
#include "../module/exglobal.h"

LogView::LogView(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LogView)
{
    ui->setupUi(this);
   // display_data();
}

LogView::~LogView()
{
    delete ui;
}

//void LogModel::setLogIndex(int index)
//int LogModel::getLogIndex(void)

void LogView::display_data(void)
{
    int log_num;
    QString record;
   // log_num=ExGlobal::pLogModel->rowCount();
    ExGlobal::pLogModel->InitLog();
    log_num=ExGlobal::pLogModel->getLogNum();
    qDebug()<<"LogView--log_num=="<<log_num;
    for(int ii=0;ii<log_num;ii++)
    {
      //  qDebug()<<"LogView--ii=="<<ii;
       ExGlobal::pLogModel->setCurrLog(ii);
       record=QString::number(ExGlobal::pLogModel->getCurrLodID(),10);
       record.resize(6,' ');
      //  qDebug()<<"LogView--getCurrLodID=="<<ExGlobal::pLogModel->getCurrLodID();
       record=record+ExGlobal::pLogModel->getCurrLogType();
       record.resize(14,' ');
       // qDebug()<<"LogView--getCurrLogType=="<<ExGlobal::pLogModel->getCurrLogType();
       record=record+ExGlobal::pLogModel->getCurrLogResult();
        record.resize(22,' ');
      // qDebug()<<"LogView--getCurrLogResult=="<<ExGlobal::pLogModel->getCurrLogResult();
       record=record+ExGlobal::pLogModel->getCurrValidDateTime();
       record.resize(42,' ');
        // qDebug()<<"LogView--getCurrValidDateTime=="<<ExGlobal::pLogModel->getCurrValidDateTime();

       record=record+ExGlobal::pLogModel->getCurrLogMsg();
       record.resize(70,' ');
       // qDebug()<<"LogView--getCurrLogMsg=="<<ExGlobal::pLogModel->getCurrLogMsg();
         // qDebug()<<"LogView--getCurrValidDateTime=="<<ExGlobal::pLogModel->getCurrValidDateTime();
       ui->ltWidget_log->addItem(record);
    }
     ui->ltWidget_log->setCurrentRow(0);
}

void LogView::refresh_data(void)
{
    int log_num;
    QString record;
    ui->ltWidget_log->clear();
   // log_num=ExGlobal::pLogModel->rowCount();
   // ExGlobal::pLogModel->InitLog();
    log_num=ExGlobal::pLogModel->getLogNum();
    qDebug()<<"LogView--log_num=="<<log_num;
    for(int ii=0;ii<log_num;ii++)
    {
      //  qDebug()<<"LogView--ii=="<<ii;
       ExGlobal::pLogModel->setCurrLog(ii);
       record=QString::number(ExGlobal::pLogModel->getCurrLodID(),10);
       record.resize(6,' ');
   //     qDebug()<<"LogView--getCurrLodID=="<<ExGlobal::pLogModel->getCurrLodID();
       record=record+ExGlobal::pLogModel->getCurrLogType();
       record.resize(14,' ');
     //   qDebug()<<"LogView--getCurrLogType=="<<ExGlobal::pLogModel->getCurrLogType();
       record=record+ExGlobal::pLogModel->getCurrLogResult();
        record.resize(22,' ');
     //  qDebug()<<"LogView--getCurrLogResult=="<<ExGlobal::pLogModel->getCurrLogResult();
        record=record+ExGlobal::pLogModel->getCurrValidDateTime();
        record.resize(42,' ');
        // qDebug()<<"LogView--getCurrValidDateTime=="<<ExGlobal::pLogModel->getCurrValidDateTime();

        record=record+ExGlobal::pLogModel->getCurrLogMsg();
       record.resize(70,' ');
       record=record.mid(0,70);
        qDebug()<<"LogView--getCurrLogMsg=="<<ExGlobal::pLogModel->getCurrLogMsg();
          ui->ltWidget_log->addItem(record);
       }
    ui->ltWidget_log->setCurrentRow(0);
}

void LogView::on_comboBox_currentIndexChanged(int index)
{

}

void LogView::on_cBox_log_selest_currentIndexChanged(int index)
{
    ExGlobal::pLogModel->refresh(index);
    qDebug()<<"on_comboBox_currentIndexChanged"<<index;
    refresh_data();
}
