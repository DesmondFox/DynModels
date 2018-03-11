#include <QApplication>
#include <QDebug>
#include <QSplashScreen>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Показ картинки загрузки
    QSplashScreen splash(QPixmap(":/splashscreen/assets/splash.png"));
    qDebug() << "Showing splash screen";
    splash.show();

    MainWindow w;
    splash.finish(&w);
    w.showMaximized();

    return a.exec();
}
