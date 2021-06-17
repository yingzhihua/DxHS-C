#ifndef USBFILE_H
#define USBFILE_H

#include <QDialog>

namespace Ui {
class UsbFile;
}

class UsbFile : public QDialog
{
    Q_OBJECT

public:
    explicit UsbFile(QWidget *parent = nullptr);
    ~UsbFile();

private:
    Ui::UsbFile *ui;
};

#endif // USBFILE_H
