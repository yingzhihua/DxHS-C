#ifndef CKCAMERADRV_H
#define CKCAMERADRV_H
#include <QString>
#include "CKCameraInterface.h"
class CKCameraDrv
{
public:
    CKCameraDrv();
    static void DrvInit();
    static bool ExistFront(){return FrontExist;}
    static bool ExistBehind(){return BehindExist;}
    static bool openCamera(bool front = false);
    static bool closeCamera(bool front = false);    
    static int getImageType(bool front = false){return front?frontRawImageType:behindRawImageType;}
    static uint32_t GetFrame(uint8_t *mem, bool front = false);

    static bool setabsExpose(int value, bool front = false);
    static bool setGain(int value, bool front = false);
    static bool setResolution(int resolutionIndex, bool front = false);

private:
    static QString FrontCameraName;
    static QString BehindCameraName;
    static bool FrontExist;
    static bool BehindExist;
    static HANDLE m_frontCamera;
    static HANDLE m_behindCamera;
    static int frontRawImageType;
    static int behindRawImageType;
};

#endif // CKCAMERADRV_H
