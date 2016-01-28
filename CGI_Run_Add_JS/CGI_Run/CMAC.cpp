#include "CMAC.h"
#include <QCoreApplication>

CMAC::CMAC(QObject *parent) :
    QObject(parent)
{
    m_nChongQiShiJian = 1*60*60;///< 6个小时重新启动一下机器
    m_nStartTime = GetSysAbsTime();
#if 0
    char p[32] = {'B','E','I','J','I','N','G','W','E','I','K','O','N','G','G','O','N','G','Y','E','W','A','N','G','G','U','A','N','J','I','S','H'};
#else
    char p[32] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,
                  0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1a,0x1b,0x1c,0x1d,0x1e,0x1f};
#endif
    memcpy(myKey,p,32);
}

bool CMAC::MACIsTrue()
{
    int fd,res;
    char rsp[32];
    char mac[32];
    char random[32];
//    printf("1--------------------------------------------\n");
    fd = open("/dev/atsha0", O_RDWR);
    if(fd < 0){
        return false;
    }
//    printf("2--------------------------------------------\n");
    res = ioctl(fd,IO_SN_READ,serno);
    if(res){
        printf("error read serno\n");
        close(fd);
        return false;
    }else{
//        int i;
        printf("Serial No: ");
        hexdump(serno, 9);
    }
//    printf("3--------------------------------------------\n");
    if(getrandom(random)){
        close(fd);
        return false;
    }
//    printf("4--------------------------------------------\n");
    if(getchallenge(random,mac)){
        close(fd);
        return false;
    }
//    printf("5--------------------------------------------\n");
    if(macvalid(fd,random,rsp))
    {
        printf("MAC not Valid\n");
        close(fd);
        return false;
    }
//    printf("6--------------------------------------------\n");
    bool result = false;
    if(!memcmp(mac,rsp,32))
    {
        printf("MAC is Valided\n");
        result = true;
    }else
    {
        printf("MAC Valid error\n");
        result = false;
    }
    close(fd);

    return result;
}
void CMAC::hexdump(char *p, int n)
{
    int i, off;

    for (off = 0; n > 0; off += 16, n -= 16) {
        printf("%s%04x:", off == 0 ? "\n" : "", off);
        i = (n >= 16 ? 16 : n);
        do {
            printf(" %02x", *p++ & 0xff);
        } while (--i);
        printf("\n");
    }
}


int CMAC::devcrypto()
{
    static int fd = -1;

    if (fd < 0)
    {
        fd = open("/dev/crypto", O_RDWR, 0);
        if (fd < 0)
        {
            err(1, "/dev/crypto");
            return fd;
        }
        if (fcntl(fd, F_SETFD, 1) == -1)
        {
            err(1, "fcntl(F_SETFD) (devcrypto)");
            return fd;
        }
    }
    return fd;
}

int CMAC::crget()
{
    int fd;

    if (ioctl(devcrypto(), CRIOGET, &fd) == -1)
        err(1, "line %d:ioctl(CRIOGET)", __LINE__);
    if (fcntl(fd, F_SETFD, 1) == -1)
        err(1, "fcntl(F_SETFD) (crget)");
    return fd;
}

int CMAC::getrandom(char *rand)
{
    int rfd = open("/dev/hw_random", O_RDONLY);
//    int entropy;
//    int result;
    int randomDataLen = 0;
    char *myRandomData = rand;
    if(rfd < 0){
        err(1, "/dev/hw_random");
        return -1;
    }

    while (randomDataLen < 32)
    {
        int result = read(rfd, myRandomData + randomDataLen, 32 - randomDataLen);
        if (result < 0)
        {
            printf("error read random data\n");
            close(rfd);
            return -1;
        }
        randomDataLen += result;
    }
    printf("Random:");
    hexdump(myRandomData, 32);
    close(rfd);
    return 0;
}

int CMAC::MakeAuthMAC(char *rand,char *code)
{
    bzero(code,88);
    memcpy(code,myKey,32);
    memcpy(code+32,rand,32);
    code[64] = 0x8;
    code[65] = 0x40;
    code[79] = serno[8];
    code[80] = serno[4];
    code[81] = serno[5];
    code[82] = serno[6];
    code[83] = serno[7];
    code[84] = serno[0];
    code[85] = serno[1];
    code[86] = serno[2];
    code[87] = serno[3];

    printf("SHA:");
    hexdump(code, 88);
    return 0;
}

int CMAC::getchallenge(char *rand,char *mac)
{
    int cryptfd = crget();
    char text[88];
    struct session2_op sop;
    struct crypt_op cop;
//    ulong cmd = CIOCGSESSION2;
    if(MakeAuthMAC(rand,text))
        return -1;
    bzero(&sop, sizeof(sop));
    sop.mac = CRYPTO_SHA2_256;
    sop.crid = CRYPTO_FLAG_HARDWARE; // | CRYPTO_FLAG_SOFTWARE;//
    if (ioctl(cryptfd, CIOCGSESSION, &sop) < 0) {
            err(1, "CIOCGSESSION");
            close(cryptfd);
        /* hardware doesn't support algorithm; skip it */
        return -1;
    }

    cop.ses = sop.ses;
    cop.op = 0;
    cop.flags = 0;
    cop.len = sizeof(text);
    cop.src = text;
    cop.dst = 0;
    cop.mac = mac;
    cop.iv = 0;

    if (ioctl(cryptfd, CIOCCRYPT, &cop) < 0){
            err(1, "CIOCCRYPT");
            close(cryptfd);
            return -1;
    }
    printf("calcate rsp:");
    hexdump(mac, 32);
    return 0;
}


int CMAC::macvalid(int fd,char *chl,char *rsp)
{
    int res;
    struct auth_mac amac;
    memcpy(amac.chl,chl,32);
    bzero(amac.rsp,32);
    res = ioctl(fd,IO_MAC,&amac);
    if(res){
            err(1,"macvalid");
            return -1;
    }
    memcpy(rsp,amac.rsp,32);
    printf("mac rsp:");
    hexdump(rsp, 32);
    return 0;
}

void CMAC::SetRunOneHour()
{
    connect(&m_Timer,SIGNAL(timeout()),this,SLOT(Slot_timeout()));
    m_Timer.start(1000);
}
void CMAC::Slot_timeout()
{
    qDebug()<<"AAAA="<<GetSysAbsTime()<<m_nStartTime<<m_nChongQiShiJian;
    if (ABS(GetSysAbsTime()-m_nStartTime) > m_nChongQiShiJian)
    {
        qDebug()<<"BBBB="<<GetSysAbsTime()<<m_nStartTime<<m_nChongQiShiJian;
        m_nStartTime = GetSysAbsTime();
//        qDebug()<<"CCCC="<<system("reboot");
		exit(0);
       // QCoreApplication::exit();
    }
}
/*!
 * \brief  功能概述 获取绝对秒
 * \param  参数描述 无
 * \return 返回值描述 返回绝对秒
 * \author zzy
 * \date   2015/5/27
 */
int CMAC::GetSysAbsTime()
{
    ///获取绝对秒
    int nTime = QDateTime::currentDateTime().secsTo(QDateTime(QDate(1970,1,1),QTime(0,0,0,0)));
    if(nTime < 0)
    {
        nTime = -nTime;
    }
    return nTime;
}
