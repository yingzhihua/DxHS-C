#ifndef TCAMERA_H
#define TCAMERA_H

#include <QObject>

#include "imagecapture.h"
#include "ckcamera.h"

typedef enum{
    CAMERA_TYPE_V4L2,CAMERA_TYPE_DVP,CAMERA_TYPE_CK,CAMERA_EMPTY
} CAMERATYPE;

class TCamera : public QObject
{
    Q_OBJECT
public:
    explicit TCamera(QObject *parent = nullptr);
    int openCamera();
    int closeCamera();
    int stopCamera();
    bool capture(QString fileName, int nCount);
    bool preview(int viewType = 0);
    void setPreviewType(int viewType);
    void *getyData();
    int getImageType();

    int getabsExpose();
    bool setabsExpose(int value);
    int getGain();
    bool setGain(int value);
    int getWhite();
    bool setWhite(int value);
    bool saveRaw(QString filename);

    int imagetype;    
    CAMERATYPE cameraType;
signals:
    void finishCapture(QByteArray result);
    void reView(QImage img);

public slots:
    void ActionFinish(QByteArray data){emit finishCapture(data);}
    void CameraView(QImage img){emit reView(img);}

private:
    ImageCapture *imageCapture;
    CKCamera *ckCamera;
};

#endif // TCAMERA_H
