#include "CCANThread.h"
#include <QTimer>
#include <QTime>

CCANThread::CCANThread(QObject *parent) :
    QThread(parent)
{
    m_bIsConnect = false;
    skt = -1;
    connect(this,SIGNAL(signal_State(CCANThread::CCANThread_State)),this,SLOT(slot_State(CCANThread::CCANThread_State)));
//    m_nTimerID = startTimer(1000);
}

CCANThread::~CCANThread()
{
    running = 0;
}

int CCANThread::GetLinkState() const
{
    return m_bIsConnect;
}
int CCANThread::running = 1;

void CCANThread::print_usage(char *prg)
{
    fprintf(stderr, "Usage: %s [can-interface]\n", prg);
}

void CCANThread::sigterm(int signo)
{
    printf("got signal %d\n", signo);
    running = 0;
}
bool CCANThread::OpenCANChannel()
{
#if defined(PF_CAN)
    int family = PF_CAN;
#else
    int family = 0;
#endif

//    int type = SOCK_RAW, proto = CAN_RAW;

    ////这两行信号可以屏蔽kill命令，如果这两行加上则只能使用
    /// kill -9 进程号
    /// 来杀死run_CGI进程
    /// kill -9 -1       Kill all processes you can kill.
    ///
//    signal(SIGTERM, sigterm);
//    signal(SIGHUP, sigterm);

    if (skt == -1)
    {    ///指定接口索引需要调用ioctl()（比如对于没有错误检查CAN_RAW套接字)

        if ((skt = socket(family, SOCK_RAW, CAN_RAW)) < 0)
        {
    //        perror("socket");
            return false;
        }
    }
    rfilter.can_id   = 0x82345678;
    rfilter.can_mask = 0x00000000;
    setsockopt(skt, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
//    setsockopt(skt, SOL_CAN_RAW, CAN_RAW_FILTER, NULL, 0);

    addr.can_family = family;
    strcpy(ifr.ifr_name, m_strCANName.toStdString().data());
    ioctl(skt, SIOCGIFINDEX, &ifr);
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(skt, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
//        perror("bind");
        return false;
    }
    printf("\nReceive CAN message from interface %s\n", ifr.ifr_name);
    emit signal_State(CCANThread_State_Open);
    m_bIsConnect = true;
    return true;
}

bool CCANThread::OpenCANChannel(const QString strCANName_)
{
    m_strCANName = strCANName_;
    return OpenCANChannel();
}

void CCANThread::run()
{
    char buff[BUFFER_SIZE];
    int nbytes;
    qDebug()<<"aaaaaaa1";
    while (running)
    {
        qDebug()<<"aaaaaaa2";
        memset(buff,0,BUFFER_SIZE);
        /// 发送方即便发送第28、29位为1的的帧，接收方也会将第28、29位置0。
        if ((nbytes = read(skt, &buff, BUFFER_SIZE)) < 0)
        {
            qDebug()<<"nbytes ="<<nbytes;
            emit signal_State(CCANThread_State_Close);
        }else
        {
            QByteArray data(buff,nbytes);
            emit readyRead(data);
            qDebug()<<"receive:"<<data.toHex().toUpper()<<QTime::currentTime().toString("hh:mm:ss.zzz");
        }
        qDebug()<<"aaaaaaa3";
    }
    qDebug()<<"aaaaaaa4";
}

void CCANThread::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_nTimerID)
    {
        memset(&frame,0,sizeof(frame));
        frame.can_id = 0x82345678;
            frame.data[0] = 0x01;
            frame.data[1] = 0x02;
            frame.data[2] = 0x03;
            frame.data[3] = 0x04;
            frame.data[4] = 0x05;
            frame.data[5] = 0x06;
            frame.data[6] = 0x07;
            frame.data[7] = 0xf8;
            frame.can_dlc = 8;
//        write(skt, &frame, sizeof(frame));
        char buffer[16];
        memset(buffer,0,16);
        char *pCur = buffer;
//        for (int i = 0; i < 16; ++i)
//        {
//            if (i == 4)
//            {
//                buffer[i] = 6;
//            }else
//            {
//                buffer[i] = i;
//            }
//        }
        *pCur++ = 0xFF;
        *pCur++ = 0xFF;
        *pCur++ = 0xFF;
        *pCur++ = 0x1F;

        *pCur++ = 0x06;
        *pCur++ = 0x05;
        *pCur++ = 0x06;
        *pCur++ = 0x07;

        *pCur++ = 0x08;
        *pCur++ = 0x09;
        *pCur++ = 0x0A;
        *pCur++ = 0x0B;
        *pCur++ = 0x0C;
        *pCur++ = 0x0D;
        *pCur++ = 0x0E;
        *pCur++ = 0x0F;

        char *pcur = (char*)(&frame);
        for (int i = 0; i < 16; ++i)
        {
            qDebug()<<i<<"="<<(unsigned char )(*pcur++);
        }
//        memcpy(buffer,&frame,16);
//        CANWrite(&frame,sizeof(frame));
//        CANWrite(buffer,16);
//        qDebug()<<"send frame :"<<QByteArray((char *)(&frame),16).toHex();
//        qDebug()<<"send buffer:"<<QByteArray(buffer,16).toHex().toUpper();

        myFrame.can_id = 0x12345678;
        myFrame.EFF = 1;
        myFrame.can_dlc = 8;
        CANWrite(&myFrame,sizeof(myFrame));
        qDebug()<<"send buffer:"<<QByteArray((char *)(&myFrame),sizeof(myFrame)).toHex().toUpper();

    }
}

ssize_t CCANThread::CANWrite(const void *__buf, size_t __n)
{
    if (skt >= 0)
    {
        return write(skt,__buf,__n);
    }else
    {
        return 0;
    }
}

void CCANThread::slot_State(CCANThread::CCANThread_State nstateNo_)
{
    switch (nstateNo_) {
    case CCANThread_State_Close:
        qDebug()<<"quit";
        running = 0;
        m_bIsConnect = false;
        this->quit();
        QTimer::singleShot(1000,this,SLOT(OpenCANChannel()));
        break;
    case CCANThread_State_Open:
        qDebug()<<"start";
        m_bIsConnect = true;
        this->start();
        break;
    default:
        break;
    }
}
