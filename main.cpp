#include "mainwindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


//    QList<QScreen*> list = QGuiApplication::screens();
//    for (QScreen* screen : list)
//    {
//        qDebug() << screen->size();
//    }
    return a.exec();
}
