#include "CWinClient.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    CWinClient w;
    w.showMaximized();

    return a.exec();
}
