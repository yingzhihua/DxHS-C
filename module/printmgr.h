#ifndef PRINTMGR_H
#define PRINTMGR_H
#include <QThread>
#include <QMap>
#include <QImage>

class printmgr : public QThread
{
    Q_OBJECT
public:
    explicit printmgr(QObject *parent = nullptr);
    QMap<QString,int> itemMap;
    QString testTime;
    QString validTime;
    QString user;
    QString checker;
    QString sampInfo;
    QString sampCode;
    QString sampRemark;
    QString sampType;
    QString panelName;
    bool refValid;

    bool printResult(QString dirname);
protected:
    void run();

signals:
    void finishPrint(QByteArray result);

private:
    QString printDir;
    QImage flashdxlogo;
    QImage hospitallogo;
    bool printPDF();
    bool printPTP();
    QString CreatePDF();
    bool printReport(QPainter &painter);
};

#endif // PRINTMGR_H
