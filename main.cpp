#include "WinClient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //设置界面风格
    QFile file;
    QString qss;
    file.setFileName(":/source/SoftRainBlue.css");
    file.open(QFile::ReadOnly);
    qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);

    WinClient w;
#ifdef __arm__
    w.showFullScreen();
#else
    w.show();
    w.resize(1024,768);
#endif

    return a.exec();
}
