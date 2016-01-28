#include <QCoreApplication>
#include <QTextCodec>
#include <stdio.h>
#include <time.h> //C语言的头文件
#include <sys/time.h>
#include <QDebug>
//struct tm
//{
//int tm_sec;//seconds 0-61
//int tm_min;//minutes 1-59
//int tm_hour;//hours 0-23
//int tm_mday;//day of the month 1-31
//int tm_mon;//months since jan 0-11
//int tm_year;//years from 1900
//int tm_wday;//days since Sunday, 0-6
//int tm_yday;//days since Jan 1, 0-365
//int tm_isdst;//Daylight Saving time indicator
//};
int getSystemTime()
{
    time_t timer;
    struct tm* t_tm;
    time(&timer);
    t_tm = localtime(&timer);
    printf("today is %4d%02d%02d%02d%02d%02d/n", t_tm->tm_year+1900,
    t_tm->tm_mon+1, t_tm->tm_mday, t_tm->tm_hour, t_tm->tm_min, t_tm->tm_sec);
    return 0;
}
/************************************************
设置操作系统时间
参数:*dt数据格式为"2006-4-20 20:30:30"
调用方法:
    char *pt="2006-4-20 20:30:30";
    SetSystemTime(pt);
**************************************************/
int SetSystemTime(char *dt)
{
//    struct rtc_time tm;
    struct tm tm;
    struct tm _tm;
    struct timeval tv;
    time_t timep;
    sscanf(dt, "%d-%d-%d %d:%d:%d", &tm.tm_year,
        &tm.tm_mon, &tm.tm_mday,&tm.tm_hour,
        &tm.tm_min, &tm.tm_sec);
    _tm.tm_sec = tm.tm_sec;
    _tm.tm_min = tm.tm_min;
    _tm.tm_hour = tm.tm_hour;
    _tm.tm_mday = tm.tm_mday;
    _tm.tm_mon = tm.tm_mon - 1;
    _tm.tm_year = tm.tm_year - 1900;

    timep = mktime(&_tm);
    tv.tv_sec = timep;
    tv.tv_usec = 0;
    if(settimeofday (&tv, (struct timezone *) 0) < 0)
    {
//        printf("Set system datatime error!\n");
        return -1;
    }else
    {
//        printf("Set system datatime successfully.\n");
        return 0;
    }

}
int main(int /*argc*/, char */*argv*/[])
{
    printf("Content-type: text/html;charset=utf-8\n\r\n");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(utf8);
    int len;
    char *lenstr,poststr[10000];
    lenstr = getenv("CONTENT_LENGTH");
    if(lenstr == NULL)
    {
        printf("<DIV STYLE=\"COLOR:RED\">Error parameters should be entered!</DIV>\n");
    }else
    {
        len=atoi(lenstr);
        fgets(poststr,len+1,stdin);//这里是吧从HTML传输过来的内容读取出来放到poststr中；
        QByteArray target(poststr,len);
//        qDebug()<<"target="<<"1234after"<<target;
        target.replace('+',' ');
        target.replace(QByteArray("%3A"),QByteArray(":"));
        QByteArray head("value=CurrTime&time=");
        if (target.left(head.count()) == head)
        {
            qDebug()<<"target="<<"1234before"<<target;
            QByteArray date = target.remove(0,head.count());
            qDebug()<<"date ="<<date;
            if (SetSystemTime(date.data()) == -1)
            {
                printf("0*1");
            }else
            {
#if 0
                int nfanhui = system("/sbin/hwclock -w");
                if (0 == nfanhui)/// 运行正常返回0
                {
                    printf("1");
                    qDebug()<<"zheng chang yun xing.";
                }else
                {
                    printf("0*2");
                }
#else
                pid_t status = system("/sbin/hwclock -w");
                if (-1 == status)
                {
                    printf("0*2");
                    return -1;
                }else
                {
                    if (WIFEXITED(status))
                    {
                        int errorNo = WEXITSTATUS(status);
                        if (0 == errorNo)
                        {
                            printf("1");
                            return 0;
                        }else
                        {
                            printf("0*3");
                            return -1;
                        }
                    }else
                    {
                        printf("0*4");
                        return -1;
                    }
                }
#endif
            }
        }else
        {
            printf("0*5");
        }
    }

    return 0;

//#if 1
//    QByteArray date("2015-5-6 7:8:9");
//    qDebug()<<"date ="<<date;
//    SetSystemTime(date.data());

//#else
//    time_t now; //实例化time_t结构
//    struct tm *timenow; //实例化tm结构指针
//    time(&now);
//    //time函数读取现在的时间(国际标准时间非北京时间)，然后传值给now

//    timenow = localtime(&now);
//    //localtime函数把从time取得的时间now换算成你电脑中的时间(就是你设置的地区)

//    printf("Local time is %s/n",asctime(timenow));
//    //上句中asctime函数把时间转换成字符，通过printf()函数输出

//#endif
    return 0;
}
