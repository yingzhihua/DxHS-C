#ifndef VERSION2_H
#define VERSION2_H

#include <QDialog>

namespace Ui {
class Version2;
}

class Version2 : public QDialog
{
    Q_OBJECT

public:
    explicit Version2(QWidget *parent = nullptr);
    ~Version2();

private:
    Ui::Version2 *ui;
};

#endif // VERSION2_H
