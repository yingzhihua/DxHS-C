#include "camera.h"

#include "log.h"
#include "exglobal.h"
#include "imageanalysis.h"
#include <QDebug>

static QByteArray resultData;

Camera::Camera(QObject *parent) : QObject(parent)
{
    qDebug()<<"Camera New";
    resultData.resize(12);
    resultData[0] = '\xaa';
    resultData[1] = '\x03';
    resultData[6] = '\x00';
    resultData[7] = '\xA0';
    resultData[8] = '\x00';
    resultData[9] = '\x02';
    resultData[11] = '\x55';
}

int Camera::openCamera(){
    return 0;
}

int Camera::closeCamera(){
    return 0;
}

int Camera::stopCamera(){
    return 0;
}

bool Camera::capture(QString fileName, int nCount){
    filename = fileName;
    count = nCount;
    return true;
}

bool Camera::preview(){
    resultData[10] = 0x10;
    //emit finishCapture(resultData);
    return true;
}

int Camera::getabsExpose(){
    return ExGlobal::CamAbs;
}

bool Camera::setabsExpose(int value){
    return true;
}

int Camera::getGain(){
    return ExGlobal::CamGain;
}

bool Camera::setGain(int value){
    return true;
}

int Camera::getWhite(){
    return ExGlobal::CamWhiteBlance;
}

bool Camera::setWhite(int value){
    return true;
}

double Camera::getDefinition(){
    return 0;
}

double Camera::getDefinition2(){
    return 0;
}
