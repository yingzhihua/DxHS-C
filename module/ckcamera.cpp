#include "ckcamera.h"

#include <QDebug>
#include <QDateTime>
#include <QDir>
#include <QPainter>

#include "log.h"
#include "exglobal.h"
#include "ckcameradrv.h"

static QString cameraName = "";
static QByteArray resultData;
static int imageCount = 0;
CKCamera::CKCamera(QObject *parent) : QThread(parent)
{
    rawData = new uint8_t[2592 * 1944 * 8];
    wRawData = (uint16_t *)rawData;
    wtobRawData = rawData + 2592 * 1944 * 2;
    sum = new uint32_t[2592 * 1944];
    m_hCamera = nullptr;
    stopping = false;
    count = 0;
    captureMode = CaptureMode::Idle;
    resultData.resize(12);
    resultData[0] = '\xaa';
    resultData[1] = '\x03';
    resultData[6] = '\x00';
    resultData[7] = '\xA0';
    resultData[8] = '\x00';
    resultData[9] = '\x02';
    resultData[11] = '\x55';

    Log::LogByFile("CK.txt",QString("CK Camera Init imageCount=%1").arg(imageCount));
}

bool CKCamera::readCamera(){
    int cameraNum = 0;
    CameraEnumerateDevice(&cameraNum);
    qDebug()<<"CKReadCamera count="<<cameraNum;
    Log::LogByFile("CK.txt",QString("ReadCamera count=%1").arg(cameraNum));
    if (cameraNum > 0)
    {
        tDevEnumInfo devAllInfo;
        CameraGetEnumIndexInfo(cameraNum-1, &devAllInfo);
        cameraName = tr(devAllInfo.DevAttribute.acFriendlyName);
        qDebug()<<cameraName;
        return true;
    }
    return false;
}

bool CKCamera::openCamera(){
    bool bOpen = CKCameraDrv::openCamera();
    rawImageType = CKCameraDrv::getImageType();
    return bOpen;
}

bool CKCamera::closeCamera(){
    waitStop();
    CKCameraDrv::closeCamera();        
    captureMode = CaptureMode::Idle;
    return true;
}

bool CKCamera::stopCamera(){
    qDebug()<<"CK stopCamera";
    Log::LogByFile("CK.txt",QString("stopCamera m_hCamera=%1").arg(m_hCamera!=nullptr));
    waitStop();
    qDebug()<<"CK stopCamera finish";
    captureMode = CaptureMode::Idle;
    return true;
}

uint32_t CKCamera::CKGetFrame(){
    uint32_t imagelen = CKCameraDrv::GetFrame(rawData);
    if (imagelen > 0){
        imageCount++;
        if (CKCameraDrv::getImageType() == 1){
            for (int i = 0; i < 2592*1944; i++){
                wtobRawData[i] = wRawData[i]>>4;
            }
        }
    }
    return imagelen;
}

int CKCamera::process_image(int index, uint32_t datalength){
    QString dirName = "CK";
    int width = 2592;
    int height = 1944;

    uint16_t *wData = (uint16_t *)rawData;
/*
    if (index == count){
        QImage img(bufy, width, height, QImage::Format_Grayscale8);
        img.save(Log::getDir()+"/"+dirName+"/"+filename+".bmp");
        Log::LogByFile(dirName+".txt","save file:"+Log::getDir()+"/"+dirName+"/"+filename+".bmp");
    }
*/
    if (ExGlobal::bChildImage){
        QString savefile = QString("%1/%2-%3.raw").arg(Log::getDir()).arg(filename).arg(index);
        QFile file(savefile);
        file.open(QIODevice::WriteOnly);
        file.write((const char *)rawData,datalength);
        file.close();
    }
    Log::LogByFile("CK.txt",QString("process_image,count=%1,index=%2").arg(count).arg(index));
    if (count != 1)
    {
        if (index == 1)
        {
            for (int j = 0; j < width*height; j++)
                if (rawImageType == 0)
                    sum[j] = rawData[j];
                else
                    sum[j] = wData[j];
        }
        else {
            for (int j = 0; j < width*height; j++)
                if (rawImageType == 0)
                    sum[j] += rawData[j];
                else
                    sum[j] += wData[j];
            if (count == index){
                for (int j = 0; j < width*height; j++)
                    if (rawImageType == 0)
                        rawData[j] = sum[j]/count;
                    else
                        wData[j] = sum[j]/count;
            }
        }
    }

    if (index == count){
        if (filename != ""){
            QString savefile = QString("%1/%2.raw").arg(Log::getDir()).arg(filename);
            QFile file(savefile);
            file.open(QIODevice::WriteOnly);
            file.write((const char *)rawData,datalength);
            file.close();
        }

/*
        savefile = QString("%1/%2.bmp").arg(Log::getDir()).arg(filename);
        if (rawImageType == 0){
            QImage img(rawData, width, height, QImage::Format_Grayscale8);
            img.save(savefile);
        }
        else {
            for (int i = 0; i < 2592*1944; i++){
                wtobRawData[i] = wData[i]*255/4096;
            }
            QImage image(wtobRawData, 2592, 1944, QImage::Format_Grayscale8);
            image.save(savefile);
        }

        Log::LogByFile("CK.txt","save file:"+savefile);
        */
        resultData[10] = 0;
        emit finishCapture(resultData);
    }

    return 0;
}

void CKCamera::saveRaw(void *data, uint32_t datalength){
    QString savefile = QString("%1/test.raw").arg(Log::getDir());
    QFile file(savefile);
    file.open(QIODevice::WriteOnly);
    file.write((const char *)data,datalength);
    file.close();
}

bool CKCamera::saveRaw(QString fileName){
    uint32_t datalength = 0;
    if (rawImageType == 0)
        datalength = 2592*1944;
    else
        datalength = 2592*1944*2;
    QFile file(fileName);
    file.open(QIODevice::WriteOnly);
    file.write((const char *)rawData,datalength);
    file.close();
    return true;
}

bool CKCamera::capture(QString fileName, int nCount){
    qDebug()<<"CK capture"<<captureMode;
    Log::LogByFile("CK.txt",QString("capture"));
    waitStop();
    filename = fileName;
    count = nCount;    
    openCamera();
    captureMode = CaptureMode::Capture;
    stopping = false;
    this->start();
    return true;
}

bool CKCamera::preview(int viewType){
    qDebug()<<"CK preview"<<captureMode;
    Log::LogByFile("CK.txt",QString("preview"));
    if (captureMode == CaptureMode::View)
        return true;        
    waitStop();
    openCamera();
    count = 1;
    stopping = false;
    captureMode = CaptureMode::View;
    ViewType = viewType;
    this->start();
    return true;
}

bool CKCamera::setabsExpose(int value){
    if (CKCameraDrv::setabsExpose(value)){
        ExGlobal::updateCaliParam("CamAbs",value);
        return true;
    }
    return false;
}

bool CKCamera::setGain(int value){
    if (CKCameraDrv::setGain(value))
    {
        ExGlobal::updateCaliParam("CamGain",value);        
        return true;
    }
    return false;
}

bool CKCamera::setWhite(int value){
    qDebug()<<"setWhite:"<<value;
    return true;
}

void CKCamera::run(){
    QTime time;
    int elapse;
    int nFrame = 0;
    uint32_t dataLength;
    qDebug()<<"CK run"<<stopping<<count;
    while(!stopping&&nFrame<count){
        time.start();
        dataLength = CKGetFrame();
        if (dataLength != 0){
            if (captureMode == CaptureMode::View){
                qDebug()<<"View,rawImageType="<<rawImageType;
                QPainter painter;
                if (rawImageType == 0)
                {
                    QImage image(rawData, 2592, 1944, QImage::Format_Grayscale8);
                    painter.begin(&image);
                    painter.setPen(QPen(QColor(255,255,255),10));
                    painter.drawLine(1296-20,972,1296+20,972);
                    painter.drawLine(1296,972-20,1296,972+20);
                    painter.setPen(QPen(QColor(255,255,255),4));
                    if (ViewType == 1)
                        painter.drawRect(ExGlobal::FocusX,ExGlobal::FocusY,ExGlobal::FocusWidth,ExGlobal::FocusHeight);
                    else if(ViewType == 2)
                        painter.drawEllipse(QRect(ExGlobal::LightCX-ExGlobal::LightR,ExGlobal::LightCY-ExGlobal::LightR,ExGlobal::LightR*2,ExGlobal::LightR*2));
                        //painter.drawRect(ExGlobal::LightX,ExGlobal::LightY,ExGlobal::LightWidth,ExGlobal::LightHeight);
                    painter.end();
                    emit reView(image);
                }
                else{
                    QImage image(wtobRawData, 2592, 1944, QImage::Format_Grayscale8);
                    painter.begin(&image);
                    painter.setPen(QPen(QColor(255,255,255),10));
                    painter.drawLine(1296-40,972,1296+40,972);
                    painter.drawLine(1296,972-40,1296,972+40);
                    painter.setPen(QPen(QColor(255,255,255),4));
                    if (ViewType == 1)
                        painter.drawRect(ExGlobal::FocusX,ExGlobal::FocusY,ExGlobal::FocusWidth,ExGlobal::FocusHeight);
                    else if(ViewType == 2)
                        painter.drawEllipse(QRect(ExGlobal::LightCX-ExGlobal::LightR,ExGlobal::LightCY-ExGlobal::LightR,ExGlobal::LightR*2,ExGlobal::LightR*2));                        
                    painter.end();
                    emit reView(image);
                }
            }
            else if (captureMode == CaptureMode::Capture)
            {
                nFrame++;
                process_image(nFrame,dataLength);
                elapse = time.elapsed();
                qDebug()<<"CKCamera::run,elapse="<<elapse<<"Frame="<<nFrame;
                int nFrameMs = ExGlobal::CamAbs + 400;
                if (nFrame<count && elapse < nFrameMs){
                    QThread::msleep(nFrameMs-elapse);
                }
            }
        }
        else {
            resultData[10] = 1;
            emit finishCapture(resultData);
            break;
        }

    }
    captureMode = CaptureMode::Idle;
    stopping = false;
}

bool CKCamera::waitStop(){
    stopping = true;
    for (int i = 0; i < 30; i++){
        if (!this->isRunning())
            return true;
        QThread::msleep(100);
    }
    return false;
}

void *CKCamera::getyData(){
    //if (rawImageType == 0)
        return rawData;
    //return wtobRawData;
}
