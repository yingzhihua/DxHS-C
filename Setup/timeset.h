#ifndef TIMESET_H
#define TIMESET_H

#include <QDialog>

namespace Ui {
class TimeSet;
}

class TimeSet : public QDialog
{
    Q_OBJECT

public:
    explicit TimeSet(QWidget *parent = nullptr);
    ~TimeSet();

private:
    Ui::TimeSet *ui;
};

#endif // TIMESET_H