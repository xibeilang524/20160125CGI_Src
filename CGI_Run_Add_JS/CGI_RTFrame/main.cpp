#include <QCoreApplication>
#include <QTextCodec>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include "CRTFrameRead.h"
int main(int /*argc*/, char */*argv*/[])
{
    printf("Content-type: text/html;charset=utf-8\n\r\n");
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(utf8);

#if 0
    CRTFrameRead cCRTFrameRead;
    QString result = cCRTFrameRead.GetFrame(QString("COM2"));
    printf("%s",result.toStdString().data());
#endif
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
//        QByteArray target(poststr,len);
//        printf("SRC=%s",target.data());
//        target.replace(QByteArray("%40"),QByteArray("@"));
//        target.replace(QByteArray("%2C"),QByteArray(","));
//        target.replace(QByteArray("+"),QByteArray(" "));
//        target.replace(QByteArray("%3A"),QByteArray(":"));
//        printf("HOU=%s",target.data());

        QString strInput = poststr;//post数据
//        printf("QString_SRC=%s",strInput.toStdString().data());
        printf("-----------------------------------------------------------------------------------\n");

        QStringList str_list_atsig = strInput.split("&");
//        printf("111111111\n");

//        for (int i = 0; i < str_list_atsig.count(); ++i)
//        {
//            printf("str_list_atsig.count()=%d==%s\n",i,str_list_atsig.at(i).toStdString().data());

//        }
//        printf("2222222222===%d\n",str_list_atsig.count());


//////    传输的报文
/// value=Message&channel=COM1
/// value=Message&channel=CAN1
/// value=Message&channel=Net3&link=NewLink
/// 对于串口来说只有value和channel两个属性和属性值，channel属性值也为存储实时报文共享内存的key
/// 对于CAN口来说只有value和channel两个属性和属性值,channel属性值也为存储实时报文共享内存的key
/// 对于网口来说有value、channel和link三个属性和值，其中channel代表的是哪个网卡，在这个程序中没有用到，link代表的是网络连接的名字，在这个程序中使用到了,link属性值也为存储实时报文共享内存的key
        if (str_list_atsig.count() >=2)
        {
//            printf("2222222222AA\n");
            QString str_value_Attribute;
            QString str_channel_Attribute;
            QString str_link_Attribute;
            QString str_value = str_list_atsig.at(0);
            QStringList str_value_list = str_value.split("=");
            if (str_value_list.count() >= 2)
            {
                str_value_Attribute = str_value_list.at(1);
                if ("Message" != str_value_Attribute)
                {
                    return 0;
                }
            }else
            {
                return 0;
            }

//            printf("2222222222BB\n");
            QString str_channel = str_list_atsig.at(1);
            QStringList str_channel_list = str_channel.split("=");
            if (str_channel_list.count() >= 2)
            {
//                printf("2222222222CC\n");
                str_channel_Attribute = str_channel_list.at(1);
//                printf("str_channel_Attribute=%s\n",str_channel_Attribute.toStdString().data());
                if (str_channel_Attribute.left(3) == "COM")
                {
//                    printf("CHANNELcom=%s\n",str_channel_Attribute.toStdString().data());
                    CRTFrameRead cCRTFrameRead;
                    QString result = cCRTFrameRead.GetFrame(str_channel_Attribute);
                    printf("%s",result.toStdString().data());
                }else if (str_channel_Attribute.left(3) == "CAN")
                {
//                    printf("CHANNELcan=%s\n",str_channel_Attribute.toStdString().data());
                    CRTFrameRead cCRTFrameRead;
                    QString result = cCRTFrameRead.GetFrame(str_channel_Attribute);
                    printf("%s",result.toStdString().data());
                }else if (str_channel_Attribute.left(3) == "Net")
                {
//                    printf("3333333333333333333\n");
                    if (str_list_atsig.count() >= 3)
                    {
                        QString str_link = str_list_atsig.at(2);
                        QStringList str_link_list = str_link.split("=");
//                        printf("str_linknet=%s\n",str_link.toStdString().data());
                        if (str_link_list.count() >= 2)
                        {
                            str_link_Attribute = str_link_list.at(1);
//                            printf("CHANNELnet=%s\n",str_link_Attribute.toStdString().data());
                            CRTFrameRead cCRTFrameRead;
                            QString result = cCRTFrameRead.GetFrame(str_link_Attribute);
                            printf("%s",result.toStdString().data());
                        }else
                        {
                            return 0;
                        }
                    }
                }else
                {
                    return 0;
                }
            }else
            {
                return 0;
            }


        }else
        {
            printf("if (str_list_atsig.count() <= 2)");
            return 0;
        }
    }
    return 0;
}
