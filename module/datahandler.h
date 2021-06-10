#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QObject>
#include <QPainter>
#include "opencv2/opencv.hpp"
#include "opencv2/objdetect.hpp"

using namespace cv;
using namespace std;
class DataHandler : public QObject
{
    Q_OBJECT
public:
    DataHandler();    
    static bool HandleData(int testId, QHash<int,vector<Point2d>> &posArr);
    static bool HandleDataWithStep(int testId, QHash<int,vector<Point2d>> &posArr, bool bRef, bool bSmooth, bool bBaseLine);
    static bool HandleOnePointData(int testId, QHash<int,vector<Point2d>> &posArr);
    static bool HandleOnePointDataEx(int testId, QHash<int,vector<Point2d>> &posArr);
    static bool LoadData(QString filename, QHash<int,vector<Point2d>> &posArr);
    static bool SaveData(int testId,QString strdir);
    static bool SaveOnePointData(int testId,QString strdir);
    static bool HandleFileData(QString filename,QString strdir);
    static bool debugData(QString dirname, QHash<int, vector<Point2d> > &posArr);
    static bool RawdataToMatlab(QString dirname);
    static QMap<int,int> getPosItemid();
    static void setPosItemid(uchar *data);
    static QHash<int,float> getPosIntercept();
    static void setAssayItem(QMap<int, QString> assayItem);
    static void setItemCT(QHash<int,int> ct);
    static void setItemName(QMap<QString, QString> itemName);
    static void FillTestResult(int Testid, QHash<int, vector<Point2d> > &posArr);
    static QHash<int,int> getCurrCT();
    static QHash<int,int> getCurrItemCT();
    static void printCT(QPainter &painter);
    static void printImage(QString dir, QPainter &painter);
    static int NameSize();
};

#endif // DATAHANDLER_H
