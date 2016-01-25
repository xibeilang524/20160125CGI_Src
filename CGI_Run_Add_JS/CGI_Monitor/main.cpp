#include "mainwindow.h"
#include <QApplication>
#include "CStartWindow.h"
#include "CDeviceModelMessage.h"
#include "CProtocolFileMessage.h"
#include "Src/cmyfunction.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);    

//    MainWindow w;
//    w.show();
//    CMyFunction::MonitorIPAddress = "192.168.1.110";

    CStartWindow start;
    start.show();

    return a.exec();
}
