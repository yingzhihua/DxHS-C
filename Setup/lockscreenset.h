#ifndef LOCKSCREENSET_H
#define LOCKSCREENSET_H

#include <QDialog>

namespace Ui {
class LockScreenSet;
}

class LockScreenSet : public QDialog
{
    Q_OBJECT

public:
    explicit LockScreenSet(QWidget *parent = nullptr);
    ~LockScreenSet();

private:
    Ui::LockScreenSet *ui;
};

#endif // LOCKSCREENSET_H
