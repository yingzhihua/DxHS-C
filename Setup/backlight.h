#ifndef BACKLIGHT_H
#define BACKLIGHT_H

#include <QDialog>

namespace Ui {
class BackLight;
}

class BackLight : public QDialog
{
    Q_OBJECT

public:
    explicit BackLight(QWidget *parent = nullptr);
    ~BackLight();

private slots:
    void on_horizontalSlider_valueChanged(int value);

    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::BackLight *ui;
};

#endif // BACKLIGHT_H
