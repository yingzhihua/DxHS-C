#ifndef VERSION2_H
#define VERSION2_H

#include <QDialog>

namespace Ui {
class Version2;
}

class Version2 : public QDialog
{
    Q_OBJECT

protected:
    void showEvent(QShowEvent *event);
    void hideEvent(QHideEvent *event);


public:
    explicit Version2(QWidget *parent = nullptr);
    ~Version2();

private slots:
    void on_pushButton_clicked();

private:
    Ui::Version2 *ui;
};

#endif // VERSION2_H
