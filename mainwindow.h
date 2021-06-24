#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "startup.h"
#include "login.h"
#include "idle.h"
#include "boxready.h"
#include "testprocess.h"
#include "Setup/setupmenu.h"
#include "Setup/systemname.h"
#include "Setup/backlight.h"
#include "Setup/languageset.h"
#include "Setup/adminpassword.h"
#include "Setup/lockscreenset.h"
#include "Setup/wifimenu.h"
#include "Setup/network.h"
#include "Setup/timeset.h"
#include "Setup/user.h"
#include "Setup/machinecalibration.h"
#include "Setup/camerasetup.h"
#include "Setup/testsetup.h"
#include "Setup/testloop.h"
#include "Setup/systemparam.h"
#include "Setup/projectparam.h"
#include "Setup/usbfile.h"
#include "Setup/light.h"
#include "Setup/pumpparam.h"
#include "Setup/logview.h"
#include "Setup/pidparam.h"
#include "Setup/version2.h"
#include "Setup/adduser.h"
#include "Setup/updateuser.h"

#include "Data/datamenu.h"

#include "module/exglobal.h"
#include "module/sequence.h"
#include "module/uihandler.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    virtual void timerEvent(QTimerEvent *e);

private:
    Ui::MainWindow *ui;
    void UISetup();
    Startup *start;
    Login *main_login;
    Idle *main_idle;
    BoxReady *main_boxready;
    TestProcess *main_testprocess;
    SetupMenu *setup_menu;
    SystemName *systemname;

    DataMenu *data_menu;

    ExGlobal *global;
    Sequence *sequence;
    BackLight *backlight;
    LanguageSet *languageset;
    AdminPassword *adminpassword;
    LockScreenSet *lockscreenset;
    WifiMenu *wifimenu;
    NetWork *network;
    TimeSet *timeset;
    User  *user;
    MachineCalibration *machinecalibration;
    CameraSetup *camerasetup;
    TestSetup *testsetup;
    TestLoop *testloop;
    SystemParam *systemparam;
    ProjectParam *projectparam;
    UsbFile *usbfile;
    Light  *light;
    PumpParam *pumpparam;
    LogView *logview;
    PIDParam *pidparam;
    Version2 *version2;
    AddUser *adduser;
    UpdateUser *updateuser;
    QMovie *movie;

public slots:
    void GoPage(UIHandler::PageId id);
    void StateUpdate(UIHandler::StateId id);
    void FooterNotify(bool setup,bool home,bool data);

private slots:
    void on_btSetup_clicked();
    void on_btHome_clicked();
    void on_btData_clicked();
};

#endif // MAINWINDOW_H
