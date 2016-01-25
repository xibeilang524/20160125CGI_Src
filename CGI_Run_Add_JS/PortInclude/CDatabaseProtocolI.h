/*!
 * \file    CDatabaseProtocolI.h
 * \brief   概述 CDatabaseProtocolI类的头文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/5/27
 *
 * \b       修改记录：
 * \li      2015/5/27
 *  添加注释
 */

#ifndef DATABASEPROTOCOL_H
#define DATABASEPROTOCOL_H

#include <QtCore/qglobal.h>

#if defined(DATABASEPROTOCOL_LIBRARY)
#  define DATABASEPROTOCOLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DATABASEPROTOCOLSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QObject>

class CRTDBI;
class CPointBase;
class CValueI;

/*!
 * \class  CDatabaseProtocolI
 * \brief  概述 数据库驱动接口纯虚基类
 * \author zzy
 * \date   2015/5/27
 * 详细概述
 */
class DATABASEPROTOCOLSHARED_EXPORT CDatabaseProtocolI : public QObject
{
    Q_OBJECT
public:
    virtual bool OnCreateChannel(const QString strFileName_, CRTDBI *pRealDatabase,int nNumber) = 0;
    virtual bool OnTagChange(CPointBase *pPointBase_, QVariant VarSet_) = 0;
};
extern "C" DATABASEPROTOCOLSHARED_EXPORT CDatabaseProtocolI *CreateDriver();

#endif // DATABASEPROTOCOL_H
