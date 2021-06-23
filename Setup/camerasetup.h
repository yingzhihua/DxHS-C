#ifndef CAMERASETUP_H
#define CAMERASETUP_H

#include <QDialog>

namespace Ui {
class CameraSetup;
}

class CameraSetup : public QDialog
{
    Q_OBJECT

public:
    explicit CameraSetup(QWidget *parent = nullptr);
    ~CameraSetup();
    void load_data(void);
    bool bView= false;

private slots:
    void on_pButton_backl_clicked();

    void on_pButton_open_view_clicked();

    void on_pButton_stop_view_clicked();

    void on_pButton_open_light_clicked();

    void on_pButton_close_light_clicked();

    void on_pButton_auto_focus_clicked();

    void on_pButton_time_clicked();

    void on_pButton_gain_clicked();

    void on_pButton_focus_clicked();

    void on_pButton_reset_clicked();

    void on_pButton_temp_clicked();

    void on_pButton_area_clicked();

    void on_cBox_show_box_clicked();

private:
    Ui::CameraSetup *ui;
};

#endif // CAMERASETUP_H
