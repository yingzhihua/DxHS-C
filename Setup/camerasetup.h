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

private:
    Ui::CameraSetup *ui;
};

#endif // CAMERASETUP_H
