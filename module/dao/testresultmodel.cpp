#include "testresultmodel.h"
#include "../sqlitemgr.h"
#include "../datahandler.h"
#include "../exglobal.h"

#include<QDebug>
static QMap<int,int> PosId;
static QHash<int,float> PosIntercept;
static QString tr_pannelCode;

#define UDCT 43
TestResultModel::TestResultModel(QObject *parent):QAbstractListModel (parent)
{    
    roles[RolesPosIndex] = "PosIndex";
    roles[RolesItemid] = "Itemid";
    roles[Rolecycle] = "cycle";
    roles[RoleTestValue] = "TestValue";
}

int TestResultModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_display_list.count();
}

QVariant TestResultModel::data(const QModelIndex &index, int role) const
{
    if (index.row()<0 || index.row()>=m_display_list.count())
        return QVariant();

    const TestResult &result = m_display_list[index.row()];

    if(role == RolesPosIndex)
        return result.PosIndex;
    else if(role == RolesItemid)
        return result.Itemid;
    else if(role == Rolecycle)
        return result.cycle;
    else if(role == RoleTestValue)
        return result.TestValue;
    return QVariant();
}

QHash<int, QByteArray> TestResultModel::roleNames() const
{
    return roles;
}

void TestResultModel::AddTest(const TestResult &result){
    beginInsertRows(QModelIndex(),rowCount(),rowCount());
    m_display_list<<result;
    endInsertRows();
}

void TestResultModel::setCurrItem(int id){
    currItemid = id;
    m_display_list.clear();

    QMap<int,int> posItem = DataHandler::getPosItemid();
    foreach(int posIndex,posItem.keys()){
        if (posItem[posIndex] == id){
            for (size_t i = 0; i < dataPos[posIndex].size(); i++){
                TestResult result;
                result.PosIndex = posIndex;
                result.Itemid = id;
                result.cycle = i+1;
                result.TestValue = dataPos[posIndex][i].y;
                m_display_list<<result;
            }
        }
    }
}

void TestResultModel::setCurrItemName(QString name){
    QList<int> item = ExGlobal::getBoxItemListByName(name);
    currItemName = name;
    m_display_list.clear();
    QMap<int,int> posItem = DataHandler::getPosItemid();
    foreach(int posIndex,posItem.keys()){        
        if (item.contains(posItem[posIndex])){
            currCT = ExGlobal::getItemCT(posItem[posIndex]);
            if (ExGlobal::pTestModel->getCurrResultType() == 3)
                currCT = DataHandler::getCurrItemCT()[posItem[posIndex]];
            for (size_t i = 0; i < dataPos[posIndex].size(); i++){
                TestResult result;
                result.PosIndex = posIndex;
                result.Itemid = posItem[posIndex];
                result.cycle = static_cast<int>(i)+1;
                result.TestValue = dataPos[posIndex][i].y;
                m_display_list<<result;
            }
        }
    }    
}

QVariant TestResultModel::getField(int row,QString field) const{
    if (row>=m_display_list.count())
        return QVariant();

    const TestResult &result = m_display_list[row];

    if(field == "PosIndex")
        return result.PosIndex;
    else if(field == "Itemid")
        return result.Itemid;
    else if(field == "cycle")
        return result.cycle;
    else if(field == "TestValue")
    {
        //qDebug()<<"row:"<<row<<",value:"<<dataPos[result.PosIndex][result.cycle-1].y<<",PosIndex:"<<result.PosIndex<<",cycle:"<<result.cycle;
        return dataPos[result.PosIndex][result.cycle-1].y;        
    }
    return QVariant();
}

void TestResultModel::setTestid(int id,QString panelCode){
    qDebug()<<"setTestid"<<id<<panelCode;
    Testid = id;
    tr_pannelCode = panelCode;
    if (panelCode == ExGlobal::DemoPanelCode)
        DataHandler::LoadData(QCoreApplication::applicationDirPath()+"/RawData.csv",dataPos);
    else if(panelCode.startsWith("31"))
    {
        DataHandler::HandleOnePointDataEx(Testid,dataPos);
        setCurrItem(2);
    }
    else if(panelCode.startsWith("30"))
    {
        DataHandler::HandleOnePointData(Testid,dataPos);
        setCurrItem(2);
    }
    else{
        if (ExGlobal::pTestModel->getCurrResultType() == 3)
            DataHandler::FillTestResult(Testid,dataPos);
        else
            DataHandler::HandleData(Testid,dataPos);
        }
    PosId = DataHandler::getPosItemid();
    qDebug()<<"setTestid,PosId"<<PosId;
    PosIntercept = DataHandler::getPosIntercept();
}

//被getItemResultByName代替，现未使用。
int TestResultModel::getItemResultDel(int testid, int itemid){
    if (testid == Testid)
    {
        foreach(int dataKey, PosId.keys()){            
            if (PosId[dataKey] == itemid && dataPos[dataKey].size()>20){
                //qDebug()<<"getItemResult"<<dataKey<<itemid<<ExGlobal::getItemCT(itemid);
                //float intercept = PosIntercept[dataKey]*0.1;
                for(int i = 10; i < dataPos[dataKey].size(); i++){
                    //qDebug()<<dataPos[dataKey][i-1].y<<ExGlobal::getItemCT(itemid)<<dataPos[dataKey][i].y;
                    if (dataPos[dataKey][i-1].y < ExGlobal::getItemCT(itemid) && dataPos[dataKey][i].y >= ExGlobal::getItemCT(itemid))
                    //if (dataPos[dataKey][i-1].y < intercept && dataPos[dataKey][i].y >= intercept)
                        return i;
                    }
            }
        }
    }
    return 0;
}

bool TestResultModel::testValidCheck(){
    QHash<int,int> ampItem;
    foreach(int dataKey, PosId.keys()){        
        int itemid = PosId[dataKey];
        if(itemid < 2)
            continue;
        if (dataPos[dataKey].size()>20){
            int ct = ExGlobal::getItemCT(itemid);
            for(int i = 10; i < dataPos[dataKey].size() && i < UDCT; i++){
                if (dataPos[dataKey][i-1].y < ct && dataPos[dataKey][i].y >= ct)
                    ampItem[itemid]++;
                }
        }
    }
    QHash<int,int>::iterator it;
    for (it = ampItem.begin(); it != ampItem.end(); it++)
        if (it.value() >= 2)
            return true;
    return false;
}

static int CalCtResult(int ct, vector<Point2d> data){
    int result = 0;
    double dct = static_cast<double>(ct);
    qDebug()<<"CalCtResult"<<data.size()<<ct<<data[data.size()-1].y;
    if(data.size()<10 || data[data.size()-1].y < ct)
        return 0;
    for (size_t i = data.size()-1; i > 10; i--){
        if (data[i].y < dct){
            if (i == data.size()-1)
                result = static_cast<int>((i+1)*10);
            else{
                dct = (dct - data[i].y)*10/(data[i+1].y-data[i].y);
                result = static_cast<int>(i+1)*10 + static_cast<int>(dct);
            }
            break;
        }
    }

    return result;
}

int TestResultModel::getItemResultByName(int testid, QString name){
    int result = 0;
    if (testid == Testid)
    {
        int testResultType = ExGlobal::pTestModel->getCurrResultType();
        QList<int> item = ExGlobal::getBoxItemListByName(name);
        QHash<int,int> ampItem;
        foreach(int dataKey, PosId.keys()){
            if (item.contains(PosId[dataKey]) && dataPos[dataKey].size()>20){
                int ct = ExGlobal::getItemCT(PosId[dataKey]);
                int value = testResultType == 3?DataHandler::getCurrCT()[dataKey]:CalCtResult(ct, dataPos[dataKey]);
                if (value > 0){
                    if (value < UDCT*10)
                        ampItem[PosId[dataKey]]++;
                    else
                        result = 2;
                }
            }
        }

        QHash<int,int>::iterator it;
        for (it = ampItem.begin(); it != ampItem.end(); it++)
            if (it.value() >= 2)
                return 1;
    }
    return result;
}

QString TestResultModel::getItemImgByName(int testid, QString name){
    int result = getItemResultByName(testid,name);
    //if (name == "呼吸道合胞病毒A" || name == "RSV-A")
    //    return "qrc:/images/Positive.png";
    if (result == 1)
        return ":/images/Positive.png";
    else if(result == 2)
        return ":/images/ud.png";
    return ":/images/Negative.png";
}

static int getCt(vector<Point2d> &posArr)
{
    int value = 0;
    for (int i = 0; i < posArr.size(); i++)
        if (posArr[i].y > value)
            value = posArr[i].y;
    return value/10;
}

//被getCurrItemResultByName代替，现未使用。
QList<int> TestResultModel::getCurrItemResultDel(){
    QList<int> result;
    foreach(int dataKey, PosId.keys()){
        if (PosId[dataKey] == currItemid && dataPos[dataKey].size()>20){
            int t_result = 0;
            int ct;
            if (tr_pannelCode.startsWith("3"))
                ct = getCt(dataPos[dataKey]);
            else
                ct = ExGlobal::getItemCT(currItemid);
                //ct = PosIntercept[dataKey]*0.1;
            qDebug()<<"getCurrItemResult,"<<dataKey<<"ct="<<ct;            
            for(int i = 10; i < dataPos[dataKey].size() && i < UDCT; i++)
                if (dataPos[dataKey][i-1].y < ct && dataPos[dataKey][i].y >= ct)
                {
                    if (i == 10)
                        t_result = 110;
                    else if(dataPos[dataKey][i].y == dataPos[dataKey][i-1].y)
                        t_result = (i+1)*10;
                    else
                        t_result = i*10+((ct-dataPos[dataKey][i-1].y)*10/(dataPos[dataKey][i].y-dataPos[dataKey][i-1].y));
                    break;
                }
            t_result += dataKey*1000;
            result<<t_result;
        }
    }
    std::sort(result.begin(),result.end());
    return result;
}

static QString ItemIndexName = "";
static int ItemNameIndex = 0;
QList<int> TestResultModel::getCurrItemResultByName(){
    QList<int> result;
    QList<int> item = ExGlobal::getBoxItemListByName(currItemName);
    qDebug()<<"getCurrItemResultByName"<<currItemName;
    int testResultType = ExGlobal::pTestModel->getCurrResultType();
    foreach(int itemid, item){
        int posIndex = 0;
        for (QMap<int,int>::iterator pos = PosId.begin(); pos != PosId.end(); pos++){
            if (pos.value() == itemid){
                posIndex = pos.key();
                if (dataPos[posIndex].size()>20){
                    int t_result = 0;
                    int ct;
                    if (tr_pannelCode.startsWith("3"))
                        ct = getCt(dataPos[posIndex]);
                    else
                        ct = ExGlobal::getItemCT(PosId[posIndex]);                    
                    /*
                    int value = CalCtResult(ct, dataPos[posIndex]);
                    qDebug()<<posIndex<<ct<<value;                    
                    if (value > 0)
                    {
                        if (value == 10)
                            t_result = 110;
                        else if(dataPos[posIndex][value].y == dataPos[posIndex][value-1].y)
                            t_result = (value+1)*10;
                        else
                            t_result = value*10+((ct-dataPos[posIndex][value-1].y)*10/(dataPos[posIndex][value].y-dataPos[posIndex][value-1].y));
                    }
                    */
                    if (testResultType == 3)
                        t_result = DataHandler::getCurrCT()[posIndex];
                    else
                        t_result = CalCtResult(ct, dataPos[posIndex]);
                    t_result += posIndex*1000;
                    result<<t_result;
                }
            }
        }
    }
    ItemIndexName = "";
    ItemNameIndex = 1;
    return result;
}

QString TestResultModel::getSubNameByPos(int pos){
    QString result = ExGlobal::AssayItem[PosId[pos]];
    if (result != ItemIndexName){
        ItemIndexName = result;
        ItemNameIndex = 1;
    }
    else
        ItemNameIndex++;
    result += "_"+QString::number(ItemNameIndex);
    return result;
}
