#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "startup.h"
#include "login.h"
#include "idle.h"
#include "Setup/setupmenu.h"
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
    SetupMenu *setup_menu;
    DataMenu *data_menu;

    ExGlobal *global;
    Sequence *sequence;

public slots:
    void GoPage(UIHandler::PageId id);
private slots:
    void on_btSetup_clicked();
    void on_btHome_clicked();
    void on_btData_clicked();
};

#endif // MAINWINDOW_H
