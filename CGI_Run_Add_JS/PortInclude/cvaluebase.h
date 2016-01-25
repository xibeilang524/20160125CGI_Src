/*!
 * \file    cvaluebase.h
 * \brief   概述 CValueBase类的头文件
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

#ifndef CVALUEBASE_H
#define CVALUEBASE_H
#include "CValueI.h"

#include <QMutexLocker>
#include <QObject>
#include <QTimer>

class CPointI;

/*!
 * \class  CValueBase
 * \brief  概述 值的基类
 * \author zzy
 * \date   2015/5/27
 * 详细概述
 */
class CValueBase : public CValueI
{
    Q_OBJECT
public:
    CValueBase();
    virtual ~CValueBase();
    CValueBase(int nValue_, int nTime_ = 0,int nQulity_ = 0);
    CValueBase(double nValue_, int nTime_ = 0,int nQulity_ = 0);
    CValueBase(char *pString, int nTime_ = 0,int nQulity_ = 0);

public:
    virtual void SetVarValue(const QVariant varValue_);
    virtual const QVariant  GetVarValue();

    virtual void SetWriteResult(int nControlResult_,const QString strMessage_);
    virtual bool WaitResult(QString strResultint ,int nMaxWaitMilliSecend_);
    virtual bool QtWaitForMultipleObjects(int nTimeOut_ = 1);
    virtual int GetnControlFlag();

private slots:
    void OnTimeOut();
//Data Member
public:
    QString m_strResultMessage;
    CPointI *m_pSource;
    QVariant m_varValue;///< 数据
    QTimer *m_pControlTimer;
    CONTROL m_nControlFlag;
};

#endif // CVALUEBASE_H
