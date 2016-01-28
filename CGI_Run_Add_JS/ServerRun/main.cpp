#include <QCoreApplication>
#include "CStart.h"

#if defined(Q_OS_LINUX)
#include <unistd.h>
#if defined(LONGTENGLANTIAN)
#include "../CGI_Run/CMAC.h"
#endif
#endif


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    CStart start;
#if defined(Q_OS_LINUX)
    #if defined(LONGTENGLANTIAN)
        CMAC mac;
        if (mac.MACIsTrue())
        {
        }else
        {
            mac.SetRunOneHour();
        }
    #endif
#endif
    if ( argc > 1 )
    {
    }else
    {
#if defined(Q_OS_LINUX)
        daemon(1,0);
#endif
    }
    start.SetStart();

    return a.exec();
}
