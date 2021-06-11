#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    if (argc > 1 && strcmp(argv[1],"debug")==0)
        ExGlobal::setDebug(true);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
