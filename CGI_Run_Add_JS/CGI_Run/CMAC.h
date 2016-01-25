#ifndef CMAC_H
#define CMAC_H

#include <QObject>
#include <QTimer>
#include <QDateTime>
#include <QDebug>

#include <sys/types.h>
#include <sys/param.h>

#include <sys/time.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <paths.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <err.h>
#include <sys/ioctl.h>

#include <sys/sysctl.h>
#include <time.h>
#include <sys/time.h>

#if defined(LONGTENGLANTIAN)
#include <crypto/cryptodev.h>
#endif

#define IO_SN_READ      0x1976
#define IO_KEY_WRITE 	0x4051
#define IO_MAC 			0x9811

#define ABS(a)  (((a)<0)?(-(a)):(a))
#define ABSDEC(a,b)	(((a)>=(b)) ? ((a)-(b)):((b)-(a)))

struct auth_mac
{
    char chl[32];
    char rsp[32];
};
class CMAC : public QObject
{
    Q_OBJECT
public:
    explicit CMAC(QObject *parent = 0);
    bool MACIsTrue();

    void SetRunOneHour();

    int GetSysAbsTime();
    int m_nChongQiShiJian;
    int m_nStartTime;
    QTimer m_Timer;
signals:

private slots:
    void Slot_timeout();
private:
    char serno[32];
    char myKey[33];
#if 0
    #if 0
        char myKey[32] = {'B','E','I','J','I','N','G','W','E','I','K','O','N','G','G','O','N','G','Y','E','W','A','N','G','G','U','A','N','J','I','S','H'};
    #else
        char myKey[32] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
                          0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f};
    #endif
#endif
    int macvalid(int fd, char *chl, char *rsp);
    int getchallenge(char *rand, char *mac);
    int MakeAuthMAC(char *rand, char *code);
    int getrandom(char *rand);
    int crget();
    int devcrypto();
    void hexdump(char *p, int n);
};


#endif // CMAC_H
