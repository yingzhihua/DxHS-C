#ifndef LOGVIEW_H
#define LOGVIEW_H

#include <QDialog>

namespace Ui {
class LogView;
}

class LogView : public QDialog
{
    Q_OBJECT

public:
    explicit LogView(QWidget *parent = nullptr);
    ~LogView();
    void display_data(void);
    void refresh_data(void);

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_cBox_log_selest_currentIndexChanged(int index);

    void on_pButton_machine_back_clicked();

private:
    Ui::LogView *ui;
};

#endif // LOGVIEW_H
