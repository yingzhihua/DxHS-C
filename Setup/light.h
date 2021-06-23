#ifndef LIGHT_H
#define LIGHT_H

#include <QDialog>

namespace Ui {
class Light;
}

class Light : public QDialog
{
    Q_OBJECT

public:
    explicit Light(QWidget *parent = nullptr);
    ~Light();
    void load_data(void);

private slots:
    void on_pButton_backl_clicked();

    void on_pButton_work_clicked();

    void on_pButton_hard_clicked();

    void on_pButton_set_clicked();

    void on_pButton_set_xy_clicked();

    void on_pButton_get_light_clicked();

private:
    Ui::Light *ui;
};

#endif // LIGHT_H
