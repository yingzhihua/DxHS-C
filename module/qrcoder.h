#ifndef QRCODER_H
#define QRCODER_H

#include <QThread>
#include <QImage>

#include "opencv2/videoio.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"

//#include "zbar.h"

using namespace cv;
using namespace std;
//using namespace zbar;

typedef enum{
    DECODE_MODE_QR,DECODE_MODE_PIERCE,
} DECODE_MODE;

class QRcoder : public QThread
{
    Q_OBJECT
public:
    explicit QRcoder(QObject *parent = nullptr);
    static QImage img;
    static QImage img2;
    static QImage img3;
    bool handleimage;
    bool scale;
    int binValue = 120;
    int poxValue = 2000;

    void Qr();
    void Pierce();
    void DebugQr();

protected:
    void run();

signals:
    void finishQRcode(QByteArray result);

public slots:

private:
    DECODE_MODE mode;
    QString strQr;
    int initX,initY;
    void handleImage(Mat &image);
    int pierce(Mat &image, QString &qrStr);
    QString QrDecode(Mat &image);
    bool haveliquids(Mat &image);
    bool havehole(Mat &image);
    bool haveQrcode(Mat &image);

    bool OpenCamera(int index, VideoCapture &cap);
};

#endif // QRCODER_H
