#-------------------------------------------------
#
# Project created by QtCreator 2021-06-08T13:13:12
#
#-------------------------------------------------

QT       += core gui
QT += charts network serialport sql xml printsupport

TARGET = DxHS
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    startup.cpp \    
    module/actionparser.cpp \
    module/camera.cpp \
    module/ckcamera.cpp \
    module/ckcameradrv.cpp \
    module/cvcapture.cpp \
    module/datahandler.cpp \
    module/exglobal.cpp \
    module/imageanalysis.cpp \
    module/imagecapture.cpp \
    module/log.cpp \
    module/printmgr.cpp \
    module/qrcoder.cpp \
    module/sequence.cpp \
    module/serialmgr.cpp \
    module/sqlitemgr.cpp \
    module/systemcmd.cpp \
    module/tcamera.cpp \
    module/udpserver.cpp \
    module/uihandler.cpp \
    module/dao/testmgr.cpp \
    login.cpp \
    idle.cpp \
    Setup/setupmenu.cpp \
    Data/datamenu.cpp \
    module/dao/wifimodel.cpp \
    module/dao/logmodel.cpp \
    module/dao/testmodel.cpp \
    module/dao/testresultmodel.cpp \
    module/dao/usbmodel.cpp \
    module/dao/usermgr.cpp \
    module/dao/usermodel.cpp \
    Setup/systemname.cpp \
    Setup/backlight.cpp \
    Setup/languageset.cpp \
    Setup/adminpassword.cpp \
    Setup/lockscreenset.cpp \
    Setup/wifimenu.cpp \
    Setup/network.cpp \
    Setup/timeset.cpp \
    Setup/user.cpp \
    Setup/machinecalibration.cpp \
    Setup/camerasetup.cpp \
    Setup/testsetup.cpp \
    Setup/testloop.cpp \
    Setup/systemparam.cpp \
    Setup/projectparam.cpp \
    Setup/usbfile.cpp \
    Setup/light.cpp \
    Setup/pumpparam.cpp \
    Setup/logview.cpp \
    Setup/pidparam.cpp \
    Setup/version2.cpp \
    boxready.cpp \
    components/threequery.cpp \
    testprocess.cpp \
    Setup/adduser.cpp \
    Setup/updateuser.cpp \
    Data/dataview.cpp \
    Data/dataline.cpp

HEADERS += \
        mainwindow.h \
    startup.h \    
    module/entity/assayitem.h \
    module/entity/testresult.h \
    module/entity/User.h \
    module/actionparser.h \
    module/camera.h \    
    module/ckcamera.h \
    module/ckcameradrv.h \
    module/cvcapture.h \
    module/datahandler.h \
    module/entity.h \
    module/exglobal.h \
    module/imageanalysis.h \
    module/imagecapture.h \
    module/log.h \
    module/printmgr.h \
    module/qrcoder.h \
    module/sequence.h \
    module/serialmgr.h \
    module/sqlitemgr.h \
    module/systemcmd.h \
    module/tcamera.h \
    module/udpserver.h \
    module/uihandler.h \
    module/dao/testmgr.h \
    login.h \
    idle.h \
    Setup/setupmenu.h \
    Data/datamenu.h \
    module/dao/wifimodel.h \
    module/dao/logmodel.h \
    module/dao/testmodel.h \
    module/dao/testresultmodel.h \
    module/dao/usbmodel.h \
    module/dao/usermgr.h \
    module/dao/usermodel.h \
    Setup/systemname.h \
    Setup/backlight.h \
    Setup/languageset.h \
    Setup/adminpassword.h \
    Setup/lockscreenset.h \
    Setup/wifimenu.h \
    Setup/network.h \
    Setup/timeset.h \
    Setup/user.h \
    Setup/machinecalibration.h \
    Setup/camerasetup.h \
    Setup/testsetup.h \
    Setup/testloop.h \
    Setup/systemparam.h \
    Setup/projectparam.h \
    Setup/usbfile.h \
    Setup/light.h \
    Setup/pumpparam.h \
    Setup/logview.h \
    Setup/pidparam.h \
    Setup/version2.h \
    boxready.h \
    components/threequery.h \
    testprocess.h \
    Setup/adduser.h \
    Setup/updateuser.h \
    Data/dataview.h \
    Data/dataline.h

FORMS += \
        mainwindow.ui \
    startup.ui \
    login.ui \
    idle.ui \
    Setup/setupmenu.ui \
    Data/datamenu.ui \
    Setup/systemname.ui \
    Setup/backlight.ui \
    Setup/languageset.ui \
    Setup/adminpassword.ui \
    Setup/lockscreenset.ui \
    Setup/wifimenu.ui \
    Setup/network.ui \
    Setup/timeset.ui \
    Setup/user.ui \
    Setup/machinecalibration.ui \
    Setup/camerasetup.ui \
    Setup/testsetup.ui \
    Setup/testloop.ui \
    Setup/systemparam.ui \
    Setup/projectparam.ui \
    Setup/usbfile.ui \
    Setup/light.ui \
    Setup/pumpparam.ui \
    Setup/logview.ui \
    Setup/pidparam.ui \
    Setup/version2.ui \
    boxready.ui \
    components/threequery.ui \
    testprocess.ui \
    Setup/adduser.ui \
    Setup/updateuser.ui \
    Data/dataview.ui \
    Data/dataline.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc


if(contains(DEFINES,PLUTFORM_ARM)){
INCLUDEPATH += /opt/opencv-4.1.1/include \
                /opt/opencv-4.1.1/include/opencv4 \
                /opt/opencv-4.1.1/include/opencv4/opencv2

LIBS += /opt/opencv-4.1.1/lib/libopencv_highgui.so \
        /opt/opencv-4.1.1/lib/libopencv_core.so \
        /opt/opencv-4.1.1/lib/libopencv_videoio.so \
        /opt/opencv-4.1.1/lib/libopencv_imgproc.so \
        /opt/opencv-4.1.1/lib/libopencv_objdetect.so \
        /opt/opencv-4.1.1/lib/libopencv_calib3d.so \
        /opt/opencv-4.1.1/lib/libopencv_features2d.so \
        /opt/opencv-4.1.1/lib/libopencv_flann.so \
        /opt/opencv-4.1.1/lib/libopencv_imgcodecs.so

INCLUDEPATH += /opt/zbar-0.10/include
LIBS += /opt/zbar-0.10/lib/libzbar.so

#INCLUDEPATH += /opt/DVPCamera
#LIBS += /opt/DVPCamera/libdvp.so

INCLUDEPATH += /opt/CKCameraL/include
LIBS += /opt/CKCamera/libCKCameraSDK_aarch64.so

INCLUDEPATH += /opt/KPOS/include_zh
LIBS += /opt/KPOS/libCsnPrinterLibs.so
}else{
INCLUDEPATH += /opt/opencv-4.1.1L/include \
                /opt/opencv-4.1.1L/include/opencv4 \
                /opt/opencv-4.1.1L/include/opencv4/opencv2

LIBS += /opt/opencv-4.1.1L/lib/libopencv_highgui.so \
        /opt/opencv-4.1.1L/lib/libopencv_core.so \
        /opt/opencv-4.1.1L/lib/libopencv_videoio.so \
        /opt/opencv-4.1.1L/lib/libopencv_imgproc.so \
        /opt/opencv-4.1.1L/lib/libopencv_objdetect.so \
        /opt/opencv-4.1.1L/lib/libopencv_calib3d.so \
        /opt/opencv-4.1.1L/lib/libopencv_features2d.so \
        /opt/opencv-4.1.1L/lib/libopencv_flann.so \
        /opt/opencv-4.1.1L/lib/libopencv_imgcodecs.so

INCLUDEPATH += /opt/zbar-0.10L/include
LIBS += /opt/zbar-0.10L/lib/libzbar.so

#INCLUDEPATH += /opt/DVPCameraL
#LIBS += /opt/DVPCameraL/libdvp.so

INCLUDEPATH += /opt/CKCameraL/include
LIBS += /opt/CKCameraL/libCKCameraSDK_x64.so

INCLUDEPATH += /opt/KPOSL/include_zh
LIBS += /opt/KPOSL/libCsnPrinterLibs.so
}
