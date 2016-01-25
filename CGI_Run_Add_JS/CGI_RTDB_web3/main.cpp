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

    QString strPoint = QString(argv[1]);
    QString PointSrc = strPoint;

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
                case QMetaType::Float:
                {
                    Result.append(QString::number(value.toFloat()));
                }
                    break;
                default:
                {
                    Result.append(QString::number(value.toUInt()));
                }
                    break;
                }
            }else
            {
                Result.append(QString::number(0));
            }
            Result.append(",");
        }
        printf("%s",Result.toStdString().data());
    }else
    {
        printf("dakaishibai\n");
        return 0;
    }
    return 0;
}
