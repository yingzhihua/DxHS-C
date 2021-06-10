#include "ckcameradrv.h"
#include <QDebug>

#include "log.h"
#include "exglobal.h"

bool CKCameraDrv::BehindExist = false;
bool CKCameraDrv::FrontExist = false;
QString CKCameraDrv::FrontCameraName = "";
QString CKCameraDrv::BehindCameraName = "";

HANDLE CKCameraDrv::m_frontCamera = nullptr;
HANDLE CKCameraDrv::m_behindCamera = nullptr;
int CKCameraDrv::frontRawImageType = 0;
int CKCameraDrv::behindRawImageType = 0;

static int frontIndex = 0;
static int behindIndex = 1;
CKCameraDrv::CKCameraDrv()
{
}

void CKCameraDrv::DrvInit(){
    int cameraNum = 0;
    QString tempName;
    CameraEnumerateDevice(&cameraNum);
    qDebug()<<"CKReadCamera count="<<cameraNum;
    Log::LogByFile("CK.txt",QString("ReadCamera count=%1").arg(cameraNum));
    for (int i = 0; i < cameraNum; i++){
        tDevEnumInfo devAllInfo;
        CameraGetEnumIndexInfo(i, &devAllInfo);
        tempName = devAllInfo.DevAttribute.acFriendlyName;
        if (tempName.startsWith("FRONT")){
            FrontExist = true;
            FrontCameraName = tempName;
            frontIndex = (i == 0 ? 0 : 1);
        }
        else{
            BehindExist = true;
            BehindCameraName = tempName;
            behindIndex = (i == 0 ? 0 : 1);
        }
        qDebug()<<tempName;
    }
    m_frontCamera = nullptr;
    m_behindCamera = nullptr;
}

bool CKCameraDrv::openCamera(bool front){
    CameraSdkStatus ret;
    tSdkCameraCapbility cap;
    HANDLE *camhandle = front?&m_frontCamera:&m_behindCamera;
    int *rawImageType = front?&frontRawImageType:&behindRawImageType;
    int cameraIndex = front?frontIndex:behindIndex;

    qDebug()<<"drv openCamera"<<"camhandle = "<<(*camhandle != nullptr)<<"front="<<front<<FrontExist<<BehindExist;
    if (*camhandle != nullptr)
        return true;
    if (front && !FrontExist)
        return false;
    if (!front && !BehindExist)
        return false;

    Log::LogByFile("CK.txt",QString("openCamera front=%1,m_hCamera=%2").arg(front).arg(*camhandle!=nullptr));
    if (*camhandle != nullptr)
        return true;

    DrvInit();

    if ((ret = CameraInit(camhandle, cameraIndex)) != CAMERA_STATUS_SUCCESS)
    {
        Log::LogByFile("CK.txt",QString("openCamera CameraInit failed,ret=%1").arg(ret));
        qDebug()<<"drv openCamera,CameraInit failed,ret="<<ret;
        *camhandle = nullptr;
        return false;
    }

    if (CameraGetCapability(*camhandle, &cap) == CAMERA_STATUS_SUCCESS){
        for (int i = 0; i < cap.tDeviceCapbility.iBayerTypeDesc; i++){
            qDebug()<<"iIndex="<<cap.tDeviceCapbility.pBayerTypeDesc[i].iIndex<<"des:"<<cap.tDeviceCapbility.pBayerTypeDesc[i].acDescription<<"MediaType:"<<cap.tDeviceCapbility.pBayerTypeDesc[i].iMediaType;
        }
        for (int i = 0; i < cap.tDeviceCapbility.iImageSizeDesc; i++){
            qDebug()<<"iIndex="<<cap.tDeviceCapbility.pImageSizeDesc[i].iIndex<<"width="<<cap.tDeviceCapbility.pImageSizeDesc[i].iWidth<<"height="<<cap.tDeviceCapbility.pImageSizeDesc[i].iHeight;
        }
    }

    *rawImageType = 0;

    if (cap.tDeviceCapbility.iBayerTypeDesc > 1)
        CameraSetSensorOutPixelFormat(*camhandle,cap.tDeviceCapbility.pBayerTypeDesc[1].iMediaType);

    uint mediaType;
    if (CameraGetSensorOutPixelFormat(*camhandle,&mediaType) == CAMERA_STATUS_SUCCESS)
    {
        qDebug()<<"mediaType:"<<mediaType;
        if (cap.tDeviceCapbility.iBayerTypeDesc > 1)
        {
            if (mediaType == cap.tDeviceCapbility.pBayerTypeDesc[1].iMediaType)
                *rawImageType = 1;
        }
    }

    double dExposureTime = front?ExGlobal::poxValue*1000:ExGlobal::CamAbs*1000;
    CameraSetIspOutFormat(*camhandle, CAMERA_MEDIA_TYPE_MONO8);
    CameraSetAeState(*camhandle,false);
    ret = CameraSetExposureTime(*camhandle,dExposureTime);

    double exposureTime = 0;
    CameraGetExposureTime(*camhandle, &exposureTime);
    qDebug()<<"Exposure Time="<<exposureTime<<"ret:"<<ret;
/*
    uint nGain = static_cast<uint>(ExGlobal::CamGain);
    nGain = nGain*100;
    if (nGain < 1000)
        nGain = 1000;
    else if (nGain > 8000)
        nGain = 8000;

    CameraSetAnalogGain(*camhandle,nGain);

    nGain = 0;
    CameraGetAnalogGain(*camhandle,&nGain);
    Log::LogByFile("CK.txt",QString("openCamera Exposure Time=%1,AnalogGain=%2").arg(exposureTime).arg(nGain));
    */
    CameraSetTriggerMode(*camhandle,1);
    CameraSetFrameSpeed(*camhandle,2);

    CameraPlay(*camhandle);    
    return true;
}

bool CKCameraDrv::closeCamera(bool front){
    HANDLE *camhandle = front?&m_frontCamera:&m_behindCamera;
    qDebug()<<"CK closeCamera"<<"front="<<front<<"camhandle="<<(*camhandle!=nullptr);
    Log::LogByFile("CK.txt",QString("closeCamera m_hCamera=%1").arg(*camhandle!=nullptr));
    if (*camhandle != nullptr)
    {
        CameraUnInit(*camhandle);
        *camhandle = nullptr;
    }
    qDebug()<<"CK closeCamera finish";
    return true;
}

uint32_t CKCameraDrv::GetFrame(uint8_t *mem, bool front){
    HANDLE *camhandle = front?&m_frontCamera:&m_behindCamera;
    uint32_t imagelen = 0;

    CameraSdkStatus status;
    HANDLE hBuf;
    uint8_t* pbyBuffer = nullptr;
    stImageInfo imageInfo;

    if (*camhandle == nullptr)
        return imagelen;
    if (front && !FrontExist)
        return imagelen;
    if (!front && !BehindExist)
        return imagelen;

    Log::LogByFile("CK.txt",QString("CKGetFrame m_hCamera=%2").arg(*camhandle!=nullptr));
    qDebug()<<"CKGetFrame"<<*camhandle;

    int GetRawTryCount = 0;
    do {
        status = CameraSoftTrigger(*camhandle);
        if (status == CAMERA_STATUS_SUCCESS)
            status = CameraGetRawImageBuffer(*camhandle, &hBuf, 2000);
        if (status != CAMERA_STATUS_SUCCESS)
            QThread::msleep(100);
        GetRawTryCount++;
        qDebug()<<"CameraGetRawImageBuffer,status="<<status<<"TryCount="<<GetRawTryCount;
    } while(GetRawTryCount < 100 && status != CAMERA_STATUS_SUCCESS);

    if (status != CAMERA_STATUS_SUCCESS)
    {
        Log::LogByFile("CK.txt",QString("CKGetFrame, CameraGetRawImageBuffer fail, status =%1").arg(status));
        return 0;
    }
    // 获取图像帧信息
    pbyBuffer = CameraGetImageInfo(*camhandle, hBuf, &imageInfo);
    imagelen = imageInfo.TotalBytes;
    memcpy(mem,pbyBuffer,imageInfo.TotalBytes);

    status = CameraReleaseFrameHandle(*camhandle, hBuf);
    if (status != CAMERA_STATUS_SUCCESS)
    {
        Log::LogByFile("CK.txt",QString("CKGetFrame, CameraReleaseFrameHandle fail, status =%1").arg(status));
        return 0;
    }

    Log::LogByFile("CK.txt",QString("CKGetFrame, complete TotalBytes =%1").arg(imageInfo.TotalBytes));
    return imagelen;
}

bool CKCameraDrv::setabsExpose(int value, bool front){
    HANDLE *camhandle = front?&m_frontCamera:&m_behindCamera;
    Log::LogByFile("CK.txt",QString("setabsExpose,value=%1").arg(value));
    if (openCamera(front))
    {
        if (CAMERA_STATUS_SUCCESS != CameraSetExposureTime(*camhandle,value*1000))
            return false;

        double exposureTime = 0;
        CameraSdkStatus ret = CameraGetExposureTime(*camhandle, &exposureTime);
        qDebug()<<"Exposure Time="<<exposureTime<<"ret:"<<ret;
        return true;
    }
    return false;
}

bool CKCameraDrv::setGain(int value, bool front){
    HANDLE *camhandle = front?&m_frontCamera:&m_behindCamera;
    qDebug()<<"setGain:"<<value;
    uint nGain = static_cast<uint>(value);
    nGain = nGain*100;
    if (nGain < 1000)
        nGain = 1000;
    else if (nGain > 8000)
        nGain = 8000;
    if (openCamera(front))
    {
        if (CAMERA_STATUS_SUCCESS != CameraSetAnalogGain(*camhandle,nGain))
            return false;

        CameraSdkStatus ret = CameraGetAnalogGain(*camhandle, &nGain);
        qDebug()<<"AnalogGain="<<nGain<<"ret:"<<ret;
        return true;
    }
    return false;
}
/****************************
 * 0:512*384
 * 1:640*480
 * 2:800*600
 * 3:1024*768
 * 6:1920*1280
 * 11:2592*1944
 * 15:1920*1080
 * *************************/
bool CKCameraDrv::setResolution(int resolutionIndex, bool front){    
    if (openCamera(front))
    {
        HANDLE *camhandle = front?&m_frontCamera:&m_behindCamera;
        tSdkImageResolution res;
        memset(&res,0,sizeof(tSdkImageResolution));
        //CameraSdkStatus ret = CameraSetResolution(*camhandle,resolutionIndex);
        res.iIndex = 255;
        res.iHOffsetFOV = 336;
        res.iVOffsetFOV = 432;
        res.iHeight = 1080;
        res.iWidth = 1920;
        res.iHeightFOV = 1080;
        res.iWidthFOV = 1920;
        CameraSdkStatus ret = CameraSetResolutionEx(*camhandle, &res);
        qDebug()<<"setResultion, CameraSetResolution"<<ret;
        if (CAMERA_STATUS_SUCCESS != ret)
            return false;
        memset(&res,0,sizeof(tSdkImageResolution));
        ret = CameraGetCurResolution(*camhandle, &res);
        qDebug()<<"Resolution="<<resolutionIndex<<"ret:"<<ret<<res.iWidth<<res.iHeight;
        return true;
    }
    return false;
}
