#include <QCoreApplication>
#include "CCGI_RTDB_Start.h"
#include <QTextCodec>
#include "CPostset.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <QDebug>
#include <QFile>
int main(int argc, char *argv[])
{
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(utf8);

    QString strResult;

    QString strPoint = QString(argv[1]);
    QString PointSrc = strPoint;
///<  点名字/值,点名字/值,点名字,值
    QStringList TagNameList;

    if (PointSrc.contains(","))
    {
        TagNameList = PointSrc.split(',');
    }else
    {
        TagNameList<<PointSrc;
    }
    qDebug()<<"TagNameList1="<<TagNameList;
    for (int i = 0; i < TagNameList.count(); ++i)
    {
        qDebug()<<__LINE__<<"i"<<i<<TagNameList.at(i).toStdString().data();
    }
    qDebug()<<__LINE__<<"GGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG";

    CCGI_RTDB_Start start;
    if (start.Open())
    {
        qDebug()<<__LINE__<<"dakaichenggong";
        for (int i = 0; i < TagNameList.count(); ++i)
        {
            bool bResult = false;

            if (TagNameList.at(i).contains("/"))
            {
                QStringList SetValueList = TagNameList.at(i).split('/');
                {
                    if (SetValueList.count() >= 2)
                    {
                        QVariant value;
                        if (SetValueList.at(1).contains("."))
                        {
                            value.setValue(SetValueList.at(1).toFloat());
                        }else
                        {
                            value.setValue(SetValueList.at(1).toInt());
                        }
                        bResult = start.SetValue(SetValueList.at(0),value);
                    }
                }
            }else
            {
                continue;
            }
            strResult.append(TagNameList.at(i));
            strResult.append(QString("(%1)").arg(bResult));
        }
    }else
    {
        printf("dakaishibai\n");
        return 0;
    }
    qDebug()<<"she zhi jie guo.\n"<<strResult;
    return 0;
}
