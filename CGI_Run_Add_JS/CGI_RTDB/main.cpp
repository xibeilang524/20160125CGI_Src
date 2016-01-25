#include <QCoreApplication>
#include "CCGI_RTDB_Start.h"
#include <QTextCodec>
#include "CPostset.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <QDebug>
#include <QFile>
int main(int /*argc*/, char */*argv*/[])
{
//    QCoreApplication a(argc, argv);

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
        QByteArray target ;
//        printf("----src=%s\n--",poststr);

        CPostset::HtmlToBytearray(poststr,len,target);
//        printf("----tar=%s\n--",target.data());

        QByteArray head("value=GetValue&point=");
        if (target.left(head.count()) == head)
        {
            qDebug()<<"xiangdeng";
        }else
        {
            qDebug()<<"buxiangdeng";
        }
#endif
        QString PointSrc = target.remove(0,head.count());
//        printf("PointSrc =%s",PointSrc.toStdString().data());
//        QString PointSrc = QString("COM2.pz@1,COM2.pz@ab,COM2.pz@ca");
//        QStringList TagNameList = PointSrc.split(',');
        QStringList TagNameList;

        if (PointSrc.contains(","))
        {
            TagNameList = PointSrc.split(',');
        }else
        {
            TagNameList<<PointSrc;
        }
        qDebug()<<"TagNameList1="<<TagNameList;
//        printf("HHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH\n");
        for (int i = 0; i < TagNameList.count(); ++i)
        {
//            printf("%d : %s\n",i,TagNameList.at(i).toStdString().data());
        }
//        printf("GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG\n");

        CCGI_RTDB_Start start;
        if (start.Open())
        {
//            printf("dakaichenggong\n");
            QString Result;
            for (int i = 0; i < TagNameList.count(); ++i)
            {
                CSharedMemoryPoint point = start.GetValue(TagNameList.at(i));
                QVariant value = point.m_nVariant;
                if (point.m_isValid)
                {
                    switch (value.type())
                    {
                    case QVariant::Bool:
                    {
                        Result.append(QString::number(value.toInt()));
                    }
                        break;
                    case QVariant::UInt:
                    {
                        Result.append(QString::number(value.toUInt()));
                    }
                        break;
                    case QVariant::Int:
                    {
                        Result.append(QString::number(value.toInt()));
                    }
                        break;
                    case QVariant::Double:
                    {
                        Result.append(QString::number(value.toDouble()));
                    }
                        break;
                    default:
                    {
                        Result.append(QString::number(value.toDouble()));
                    }
                        break;
                    }
                }else
                {
                    Result.append(QString::number(0));
                }

//                QString aaa = QString().append(QString::number(point.m_isValid)).append("__").append(QString::number(point.m_nPointIndex)).append("--").append(point.m_nVariant.toString()).append("--").append(point.m_strDateTime);
                Result.append(",");
//                bool m_isValid;///< 是否有效
//                int m_nPointIndex;///< 点的索引号
//                QVariant m_nVariant;///< 点的值
//                QString m_strDateTime;///< 时标
//                printf("TagNameList i=%d, TagName=%s, Value=%d\n ",i, TagNameList.at(i).toStdString().data(), start.GetValue(TagNameList.at(i)).m_nVariant.toInt());
//                qDebug()<<"TagNameList"<<i<<TagNameList.at(i)<<start.GetValue(TagNameList.at(i)).m_nVariant;
            }
            printf("%s",Result.toStdString().data());


        }else
        {
            printf("dakaishibai\n");
            return 0;
        }
    }
    return 0;

#if 0
    }
    printf("<HR COLOR=\"blue\" align=\"left\" width=\"100\">");
    printf("<input type=\"button\" value=\"Back CGI\" onclick=\"a()\">");
    printf("</div>\n");
    printf("<script>\n");
    printf("function a(){window.location=\"../cgi.html?value1=value1&value2=value2\"}");
    printf("</script>\n");
    printf("</BODY>\n");
    printf("</HTML>\n");
    fflush(stdout);
    sleep(20);
    return 0;
#endif
//    return a.exec();
}
