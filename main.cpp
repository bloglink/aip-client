#include "CWinClient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile file;
    QString qss;

    file.setFileName(":/source/SoftRainBlue.css");
    file.open(QFile::ReadOnly);
    qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);

    CWinClient w;
    w.show();
    w.resize(1024,768);

    return a.exec();
}
