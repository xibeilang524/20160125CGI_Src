#include "CMainWindow.h"
#include <QApplication>
#include "CDownLoadMainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CMainWindow w;
    w.show();

//    CDownLoadMainWindow down;
//    down.show();

    return a.exec();
}
