#ifndef USBFILE_H
#define USBFILE_H

#include <QDialog>
#include <QCheckBox>
#include <QLabel>

namespace Ui {
class UsbFile;
}

class UsbFile : public QDialog
{
    Q_OBJECT

protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);


public:
    explicit UsbFile(QWidget *parent = nullptr);
    ~UsbFile();
    void show_file(void);
    void Checkbox_clicked(int index);
    void deleteGridItem(QLayout * layout);

    QList<QCheckBox *> boxList;
    QList<QLabel *> labelList;


private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void copyFinish(int val);

private:
    Ui::UsbFile *ui;
};

#endif // USBFILE_H
