#include <QCoreApplication>
#include <QDebug>
#include "CStart.h"
#include "Src/cmyfunction.h"
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QMutex>
#include <QDateTime>
#include <QLockFile>
#include <iostream>
#if defined(Q_OS_LINUX)
#include <unistd.h>
    #if defined(LONGTENGLANTIAN)
        #include "CMAC.h"
    #endif
#endif
#include "../PortInclude/defined.h"
//exit(0); 在指定位置退出程序
static void print_app_usage ( char const * app_name )
{
    printf( "usage: %s [OPTIONS]\n", app_name );
    printf( "\n" );
    printf( "Valid options:\n" );
    printf( "  -d                                        : run program as a daemon\n" );
    printf( "  -f <debug-file>                           : specify a config file\n" );
    printf( "  -log <DEBUG|FILE|LESS>                    : set log level, default is WARN\n" );
}
using namespace std;
QString g_strStartTime;

void myMessageOutput_File(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    QString txt;
    switch (type) {
    case QtDebugMsg:
        txt = QString("Debug: %1 (%2:%3, %4)\n").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
        fprintf(stderr, "Debug: %s <<||%s:%u, %s||>>\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1 (%2:%3, %4)\n").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
        fprintf(stderr, "Warning: %s <<||%s:%u, %s||>>\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1 (%2:%3, %4)\n").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
        fprintf(stderr, "Critical: %s <<||%s:%u, %s||>>\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1 (%2:%3, %4)\n").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
        fprintf(stderr, "Fatal: %s <<||%s:%u, %s||>>\n", localMsg.constData(), context.file, context.line, context.function);
        if (CMyFunction::m_sFile.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append))
        {
            if (CMyFunction::m_sFile.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append))
            {
                QTextStream out(&CMyFunction::m_sFile);
                out <<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz")<<txt;
                CMyFunction::m_sFile.close();
            }
        }
        abort();
    }
    QFile outFile("debuglog.txt");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}
void myMessageOutput_Debug(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s <<||%s:%u, %s||>>\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s <<||%s:%u, %s||>>\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s <<||%s:%u, %s||>>\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s <<||%s:%u, %s||>>\n", localMsg.constData(), context.file, context.line, context.function);
        QString txt = QString("Fatal: %1 (%2:%3, %4)\n").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
        if (CMyFunction::m_sFile.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append))
        {
            if (CMyFunction::m_sFile.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append))
            {
                QTextStream out(&CMyFunction::m_sFile);
                out <<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz")<<txt;
                CMyFunction::m_sFile.close();
            }
        }
        abort();
    }
}

void myMessageOutput_LessDebug(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    fprintf(stderr, "%s\n", localMsg.constData());
    switch (type) {
    case QtDebugMsg:
//        fprintf(stderr, "Debug: %s <<||%s:%u, %s||>>\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
//        fprintf(stderr, "Warning: %s <<||%s:%u, %s||>>\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
//        fprintf(stderr, "Critical: %s <<||%s:%u, %s||>>\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
//        fprintf(stderr, "Fatal: %s <<||%s:%u, %s||>>\n", localMsg.constData(), context.file, context.line, context.function);
        QString txt = QString("Fatal: %1 (%2:%3, %4)\n").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function);
        if (CMyFunction::m_sFile.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append))
        {
            if (CMyFunction::m_sFile.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append))
            {
                QTextStream out(&CMyFunction::m_sFile);
                out <<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz")<<"Fatal:"<<txt;
                CMyFunction::m_sFile.close();
            }
        }
        abort();
    }
}
void myMessageOutput_NoDebug(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    switch (type) {
    case QtDebugMsg:
//        fprintf(stderr, "Debug: %s <<||%s:%u, %s||>>\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtWarningMsg:
//        fprintf(stderr, "Warning: %s <<||%s:%u, %s||>>\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtCriticalMsg:
//        fprintf(stderr, "Critical: %s <<||%s:%u, %s||>>\n", localMsg.constData(), context.file, context.line, context.function);
        break;
    case QtFatalMsg:
//        fprintf(stderr, "Fatal: %s <<||%s:%u, %s||>>\n", localMsg.constData(), context.file, context.line, context.function);
        QString txt = QString("Fatal: %1 (%2:%3, %4)\n").arg(msg.toLocal8Bit().constData()).arg(context.file).arg(context.line).arg(context.function);
        if (CMyFunction::m_sFile.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append))
        {
            if (CMyFunction::m_sFile.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append))
            {
                QTextStream out(&CMyFunction::m_sFile);
                out <<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz")<<txt;
                CMyFunction::m_sFile.close();
            }
        }
        abort();
    }
}
/*!
 \brief 循环遍历删除path目录中的所有文件夹和文件

 \fn CMainWindow::DeletePath
 \param path 目录名
 \return bool path是目录则返回true,不是则返回false
 \author zzy
 \date   2015/5/29
*/
bool DeleteFile(const QString & path)
{
    QDir dir(path);
    if (!dir.exists())
        return false;
    dir.setFilter(QDir::Dirs|QDir::Files);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    int i=0;
    do{
        QFileInfo fileInfo = list.at(i);
        if((fileInfo.fileName()==".")|(fileInfo.fileName()==".."))
        {
            i++;
            continue;
        }
        bool bisDir=fileInfo.isDir();
        if(bisDir)
        {
//            qDebug()<<(QString("%1 %2 %3").arg(fileInfo.size(), 10).arg(fileInfo.fileName(),10).arg(fileInfo.path()));
//            DeletePath(fileInfo.filePath());
//            dir.rmdir(fileInfo.path()+"/"+fileInfo.fileName());
        }else{
            qDebug()<<(QString("%1 %2 %3").arg(fileInfo.size(), 10).arg(fileInfo.fileName(),10).arg(fileInfo.path()));
//            if ()
            dir.remove(fileInfo.fileName());
        }
        i++;
    }while(i<list.size());

    return true;
}
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<<__DATE__<<__TIME__<<__FILE__<<__LINE__<<__func__;

#if defined(Q_OS_LINUX)
    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(utf8);
#endif
    if (CMyFunction::m_sFile.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append))
    {
        QTextStream out(&CMyFunction::m_sFile);
        QString strCommand;
        for (int i = 0; i < argc; ++i)
        {
            strCommand.append(argv[i]).append(" ");
        }
        out <<"start time: " <<QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz")<<" <"<< strCommand<< ">"<< "\n";
        CMyFunction::m_sFile.close();
    }
    QLockFile file(QString(LOCKFILE));
    if (file.tryLock())
    {

    }else
    {
        QString message = QString(LOCKFILE);
        qDebug()<<message<<"has lock.";
        return -3;
    }
    char const * log_level = NULL;
//    bool run_as_deamon = true;
    if ( argc > 1 )
    {
        int index;
        bool param_error = false;

        for ( index = 1; index < argc; index++ )
        {
            /*if ( strcmp( argv[ index ], "-d" ) == 0 )
            {
                run_as_deamon = false;
            }
            else */if ( strcmp( argv[ index ], "-log" ) == 0 )
            {
                index++;
                if ( index < argc )
                {
                    if ( strcmp( argv[ index ], "DEBUG" ) == 0
                         ||strcmp( argv[ index ], "FILE" ) == 0
                         ||strcmp( argv[ index ], "LESS" ) == 0)
                    {
                        log_level = argv[ index ];
                    }
                    else
                    {
                        param_error = true;
                        break;
                    }
                }
                else
                {
                    param_error = true;
                    break;
                }
            }
            else
            {
                param_error = true;
                break;
            }
        }

        if ( param_error )
        {
            print_app_usage( argv[ 0 ] );
            return -1;
        }
//        if ( run_as_deamon )
//        {
//#if defined(Q_OS_LINUX)
//            daemon(1,0);
//#endif
//        }
    }else
    {
#if defined(Q_OS_LINUX)
            daemon(1,0);
#endif
    }

    qDebug()<<"Start";
#if defined(Q_OS_LINUX)
    if (log_level != NULL)
    {
        QString command(log_level);
        if (command == "FILE")
        {
            g_strStartTime = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmsszzz")+"log.txt";
            qInstallMessageHandler(myMessageOutput_File);///上电自启动时如果qdebug信息的话就运行不起来，然后将qdebug重定向就能够正常上电自启动了，不知道原因是什么
        }else if (command == "DEBUG")
        {
            g_strStartTime = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmsszzz")+"log.txt";
            qInstallMessageHandler(myMessageOutput_Debug);///上电自启动时如果qdebug信息的话就运行不起来，然后将qdebug重定向就能够正常上电自启动了，不知道原因是什么
        }else if (command == "LESS")
        {
            g_strStartTime = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmsszzz")+"log.txt";
            qInstallMessageHandler(myMessageOutput_LessDebug);///上电自启动时如果qdebug信息的话就运行不起来，然后将qdebug重定向就能够正常上电自启动了，不知道原因是什么
        }
    }else
    {
        g_strStartTime = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmsszzz")+"log.txt";
        qInstallMessageHandler(myMessageOutput_NoDebug);///上电自启动时如果qdebug信息的话就运行不起来，然后将qdebug重定向就能够正常上电自启动了，不知道原因是什么
    }
#endif

#if defined(Q_OS_LINUX)
#if defined(LONGTENGLANTIAN)
    CMAC mac;
    if (mac.MACIsTrue())
    {
    }else
    {
        mac.SetRunOneHour();
    }
#endif
#endif

#if defined(Q_PROCESSOR_ARM)
    system(RMQIPC_SHAREDMEMORY);/// 清除共享内存文件
#endif

    CStart start;


    return a.exec();
}
