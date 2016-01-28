/*!
 * \file    CDatabaseThread.h
 * \brief   概述 CDatabaseThread类头文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/2/4
 *
 * \b       修改记录：
 * \li      2015/2/4
 *  添加注释
 */

#ifndef CDATABASETHREAD_H
#define CDATABASETHREAD_H

#include <QThread>
#include <QLibrary>
#include "Src/crtdbbase.h"
class CDatabaseProtocolI;

/*!
 * \class  CDatabaseThread
 * \brief  概述 数据库线程类
 * \author zzy
 * \date   2015/2/4
 * 详细概述
 */
class CDatabaseThread : public QThread
{
    Q_OBJECT
public:
    explicit CDatabaseThread(CRTDBBase *pRealTimeDB_,const QString &strFileName_,int nNumber,QObject *parent = 0);
    void run() Q_DECL_OVERRIDE;
private:
    CRTDBBase *m_pRealTimeDB;
    CDatabaseProtocolI *m_pDatabaseProtocol;
    QString m_strFileName;
    QString m_strLibFilePath;
    QLibrary mylib;
    bool m_IsStart;
//    int m_nSaveType;///< 保存类型 周期存储和变化存储
    int m_nNumber;///< 数据库编号,此数对于每个数据库驱动都是唯一的,解决了一个应用程序打开多个数据库的问题
};

#endif // CDATABASETHREAD_H
