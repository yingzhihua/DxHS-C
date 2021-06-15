#ifndef UIHANDLER_H
#define UIHANDLER_H

#include <QObject>

class UIHandler : public QObject
{
    Q_OBJECT
public:
    enum PageId{
        Page_Main,
        Page_Main_Startup,
        Page_Main_Login,
        Page_Main_Idle,
        Page_Main_BoxReady,
        Page_Main_Test,
        Page_Setup,
        Page_Setup_SystemName,
        Page_Setup_LockScreen,
        Page_Setup_Backlight,
        Page_Setup_Language,
        Page_Setup_Password,
        Page_Setup_Wifi,
        Page_Setup_Network,
        Page_Setup_Time,
        Page_Setup_User,
        Page_Setup_Machine,
        Page_Setup_Camera,
        Page_Setup_TestSetup,
        Page_Setup_TestLoop,
        Page_Setup_SystemParam,
        Page_Setup_ProjectParam,
        Page_Setup_UsbFile,
        Page_Setup_Light,
        Page_Setup_PumpParam,
        Page_Setup_Log,
        Page_Setup_PID,
        Page_Setup_Version,
        Page_Data,
        Page_Data_View,
        Page_Data_InvalidView,
        Page_Data_Line,
        Page_Data_OneDataLine,
        Page_Empty
    };

    explicit UIHandler(QObject *parent = nullptr);
    static UIHandler *getPtr();
    static void GoPage(PageId id);

signals:
    void Go(PageId id);

public slots:
};

#endif // UIHANDLER_H
