#ifndef ERRPAGE_H
#define ERRPAGE_H

#include <QDialog>

namespace Ui {
class ErrPage;
}

class ErrPage : public QDialog
{
    Q_OBJECT

public:
    explicit ErrPage(QWidget *parent = nullptr);
    void setError(QString error);
    ~ErrPage();

private:
    Ui::ErrPage *ui;
};

#endif // ERRPAGE_H
