#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>

#include "cameraplayer.h"
#include "imagecapture.h"

class Camera : public QObject
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = nullptr);
    enum class CaptureMode{
        Idle,
        View,
        Capture
    };
    Q_ENUM(CaptureMode)

    int openCamera();
    int closeCamera();
    int stopCamera();
    bool capture(QString fileName, int nCount);
    bool preview();
    void *getyData(){return nullptr;}

    int getabsExpose();
    bool setabsExpose(int value);
    int getGain();
    bool setGain(int value);
    int getWhite();
    bool setWhite(int value);
    double getDefinition();
    double getDefinition2();

    int imagetype;
    CaptureMode captureMode;

signals:
    void finishCapture(QByteArray result);
    void reView(QImage img);

private:
    CAMERATYPE cameraType;
    ImageCapture *imageCapture;
    CameraPlayer *camera;
    QString filename;
    int count;
};

#endif // CAMERA_H
