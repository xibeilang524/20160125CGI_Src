#include <QCoreApplication>
#include <QTextCodec>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <QDebug>
#include <QFile>
#include "CHDBRead.h"
#include <QDateTime>
/** *
 * operate=select&value=COM2.pz@1&starttime=2015-11-04 00:00:00&endtime=2015-11-29 00:00:00
 *
 * --operate=select&value=COM2.pz@1&starttime=2015-07-03 00:00:00&endtime=2015-11-20 00:00:00--
   ==COM2.pz@1==2015-07-03 00:00:00==2015-11-20 00:00:00
{value:[]}
* **/
int main(int /*argc*/, char */*argv*/[])
{
    printf("Content-type: text/html;charset=utf-8\n\r\n");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(utf8);
#if 1
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
        target.replace(QByteArray("%40"),QByteArray("@"));
        target.replace(QByteArray("%2C"),QByteArray(","));
        target.replace(QByteArray("+"),QByteArray(" "));
        target.replace(QByteArray("%3A"),QByteArray(":"));
//        printf("--%s--\n",target.data());

#endif
//        QByteArray target("operate=select&value=COM2.pz@1&starttime=2015-11-04 00:00:00&endtime=2015-11-29 00:00:00");
        int nvalue_StartByte = target.indexOf(QByteArray("value="));
        int nStartTime_StartByte = target.indexOf(QByteArray("&starttime="));
        int nEndTime_StartByte = target.indexOf(QByteArray("&endtime="));
//        printf("target=%s\n",target.data());
        qDebug()<<"target="<<target;
        qDebug()<<"nStartTime_StartByte ="<<nStartTime_StartByte;
        qDebug()<<"target.mid(nStartTime_StartByte+11) ="<<target.mid(nStartTime_StartByte+11,nEndTime_StartByte-nStartTime_StartByte-11);
        qDebug()<<"target.mid(nEndTime_StartByte+9) ="<<target.mid(nEndTime_StartByte+9);
        qDebug()<<"nEndTime_StartByte ="<<nEndTime_StartByte;
        QString PointSrc = QString(target.mid(nvalue_StartByte+6,nStartTime_StartByte-nvalue_StartByte-6));
        qDebug() << "PointSrc ="<<PointSrc;
        QByteArray baStartTime(target.mid(nStartTime_StartByte+11,nEndTime_StartByte-nStartTime_StartByte-11));
        QByteArray baEndTime(target.mid(nEndTime_StartByte+9));
//        printf("StartTime=%s\n",baStartTime.data());
//        printf("EndTime=%s\n",baEndTime.data());
//    }
//        printf("PointSrc= %s\n",PointSrc.toStdString().data());
//        QString PointSrc = QString("COM1.Device2@PointYX1,COM1.Device2@PointYX2,COM1.Device2@PointYC2");
        QStringList TagNameList;

        if (PointSrc.contains(","))
        {
            TagNameList = PointSrc.split(',');
        }else
        {
            TagNameList<<PointSrc;
        }
        QDateTime dtStartTime = QDateTime::fromString(QString(baStartTime),"yyyy-MM-dd hh:mm:ss");
        QDateTime dtStopTime = QDateTime::fromString(QString(baEndTime),"yyyy-MM-dd hh:mm:ss");
//        printf("==%s==%s==%s\n",PointSrc.toStdString().data(),baStartTime.data(),baEndTime.data());
        qDebug()<<"dtStartTime ="<<dtStartTime<<dtStopTime;
//        QDateTime dtStartTime(QDate(2010,10,10),QTime(0,0,0));
//        QDateTime dtStopTime(QDate(2016,10,10),QTime(0,0,0));
        CHDBRead hdread;
        if (hdread.OpenSqliteDatabase())
        {
//            printf("Open DB success.\n");
            QString Result;
            for (int i = 0; i < TagNameList.count(); ++i)
            {
//                printf("PointSrc= %d - %s\n",i,TagNameList.at(i).toStdString().data());
                Result.append(hdread.GetValue(TagNameList.at(i),dtStartTime,dtStopTime));
            }
            printf("{value:[%s]}",Result.toStdString().data());
        }else
        {
            printf("Open DB error!\n");
        }
    }
    return 0;
}
