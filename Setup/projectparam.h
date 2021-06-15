#ifndef PROJECTPARAM_H
#define PROJECTPARAM_H

#include <QDialog>

namespace Ui {
class ProjectParam;
}

class ProjectParam : public QDialog
{
    Q_OBJECT

public:
    explicit ProjectParam(QWidget *parent = nullptr);
    ~ProjectParam();

private:
    Ui::ProjectParam *ui;
};

#endif // PROJECTPARAM_H
