#ifndef SYSTEMNAME_H
#define SYSTEMNAME_H

#include <QDialog>

namespace Ui {
class SystemName;
}

class SystemName : public QDialog
{
    Q_OBJECT

public:
    explicit SystemName(QWidget *parent = nullptr);
    ~SystemName();


private slots:
    void on_commandLinkButton_clicked();

private:
    Ui::SystemName *ui;
};

#endif // SYSTEMNAME_H
