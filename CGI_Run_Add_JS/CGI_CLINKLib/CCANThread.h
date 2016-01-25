#ifndef CCANTHREAD_H
#define CCANTHREAD_H

#include <QThread>
/* Example of CAN for PISO-CAN200/400 Cards
 *
 *    Author: Golden Wang
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; either version 2, or (at your option)
 *    any later version.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software Foundation,
 *    Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

/* File level history (record changes for this file here.)
 *
 *    v 0.0.0 25 Mar 2010 by Golden Wang
 *         create, blah blah...
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <libgen.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>

#include <QDebug>
#include <QTimerEvent>
//#include "can.h"
//#include "raw.h"


// startup.sh
// ip link set can0 type can bitrate 1000000
// ip link set can1 type can bitrate 1000000
// ifconfig can0 up
// ifconfig can1 up

#ifndef MY_CAN_FRAME
#define MY_CAN_FRAME
struct My_can_frame
{
    union
    {
        struct
        {
            canid_t     can_id:29;	// 帧ID       CAN识别符 (11/29 bit)
            canid_t     ERR:1;		// 错误帧标识位
                                    // 错误帧标志 (0 = data frame, 1 = error frame) 0表示正确帧
                                    // 1表示错误帧，当帧格式为错误帧时，接收到的数据部分的值都为0，即8字节的数据都为0
            canid_t     RTR:1;		// 远程发送请求标识位  (1 = rtr frame)
            canid_t     EFF:1;		// 帧格式标志 (0 = standard 11 bit, 1 = extended 29 bit)
                                    // 0表示标准帧，帧ID占11位，即只有11位帧ID有效
                                    // 1表示扩展帧，帧ID占29位，即29位帧ID有效
        };
        canid_t CAN_ID;//可变结构限定词
    };
    __u8    can_dlc; /* data length code: 0 .. 8 */
    __u8    data[8] __attribute__((aligned(8)));
};
#endif

extern int optind, opterr, optopt;

class CCANThread : public QThread
{
    Q_OBJECT
public:
    explicit CCANThread(QObject *parent = 0);
    ~CCANThread();
    int GetLinkState() const;
    bool OpenCANChannel(const QString strCANName_);
    void run() Q_DECL_OVERRIDE ;
    enum CCANThread_State{
        CCANThread_State_Close=1,
        CCANThread_State_Open,
    };
    void timerEvent(QTimerEvent *event);

    ssize_t CANWrite(const void *__buf, size_t __n);
signals:
    void readyRead(const QByteArray baData_);
    void signal_State(CCANThread::CCANThread_State nstateNo_);
private slots:
    void slot_State(CCANThread::CCANThread_State nstateNo_);
    bool OpenCANChannel();

public slots:
private:

    QString m_strCANName;
    int skt;
    static int running;
    bool m_bIsConnect;
    enum{BUFFER_SIZE=1024};
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame;
    struct can_filter rfilter;

    My_can_frame myFrame;

    void print_usage(char *prg);
    static void sigterm(int signo);

    int m_nTimerID;
};

#endif // CCANTHREAD_H
