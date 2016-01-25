#include <QCoreApplication>
#include <QTextCodec>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <QDebug>
#include <QFile>
#include <QDateTime>
#include "CRTFrameRead.h"
int main(int argc, char *argv[])
{
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(utf8);
    printf("-----------------------------------------------------------------------------------\n");
    QString str_channel_Attribute = QString(argv[1]);
    if (str_channel_Attribute.left(3) == "COM")
    {
        printf("CHANNELcom=%s\n",str_channel_Attribute.toStdString().data());
        CRTFrameRead cCRTFrameRead;
        QString result = cCRTFrameRead.GetFrame(str_channel_Attribute);
        printf("%s",result.toStdString().data());
    }else if (str_channel_Attribute.left(3) == "CAN")
    {
        printf("CHANNELcan=%s\n",str_channel_Attribute.toStdString().data());
        CRTFrameRead cCRTFrameRead;
        QString result = cCRTFrameRead.GetFrame(str_channel_Attribute);
        printf("%s",result.toStdString().data());
    }else if (str_channel_Attribute.left(3) == "Net")
    {
        printf("3333333333333333333\n");
        QString str_link = str_channel_Attribute;
        QStringList str_link_list = str_link.split(".");
        printf("str_linknet=%s\n",str_link.toStdString().data());
        if (str_link_list.count() >= 2)
        {
            QString str_link_Attribute = str_link_list.at(1);
            printf("CHANNELnet=%s\n",str_link_Attribute.toStdString().data());
            CRTFrameRead cCRTFrameRead;
            QString result = cCRTFrameRead.GetFrame(str_link_Attribute);
            printf("%s",result.toStdString().data());
        }else
        {
            return 0;
        }
    }
    return 0;
}
