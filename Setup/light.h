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

private:
    Ui::Light *ui;
};

#endif // LIGHT_H
