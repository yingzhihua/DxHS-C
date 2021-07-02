#include "mainwindow.h"
#include <QApplication>
#include <QInputMethod>
#include <QDebug>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    if (argc > 1 && strcmp(argv[1],"debug")==0)
        ExGlobal::setDebug(true);

    QApplication a(argc, argv);

    QFile file(":/DxUI.qss");
    if (file.open(QFile::ReadOnly)){
        QString qss = file.readAll();
        a.setStyleSheet(qss);
        file.close();
    }    

    MainWindow w;
    w.show();    
    return a.exec();
}
