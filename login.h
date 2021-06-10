#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();
    
    void Init();

private slots:
    void on_btLogin_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
