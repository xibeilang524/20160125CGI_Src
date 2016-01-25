/*!
 * \file    cvaluebase.cpp
 * \brief   概述 CValueBase类的源文件
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

#include "cvaluebase.h"
#include <QCoreApplication>
#include <QDebug>
#include <QThread>

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
CValueBase::CValueBase()
{
    m_pSource = NULL;
    m_pControlTimer = NULL;
    m_nControlFlag = CONTROL_UNDEFINE;
}

/*!
 * \brief  功能概述 析构函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
CValueBase::~CValueBase()
{
}

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 nValue_是值，nTime_，nQulity_
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
CValueBase::CValueBase(int nValue_, int /*nTime_*/, int /*nQulity_*/)
{
    m_pSource = NULL;
    m_pControlTimer = NULL;
    m_nControlFlag = CONTROL_UNDEFINE;
    m_varValue.setValue(nValue_);
}

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 nValue_是值，nTime_，nQulity_
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
CValueBase::CValueBase(double nValue_, int /*nTime_*/, int /*nQulity_*/)
{
    m_pSource = NULL;
    m_pControlTimer = NULL;
    m_nControlFlag = CONTROL_UNDEFINE;
    m_varValue.setValue(nValue_);
}

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 pString是值，nTime_，nQulity_
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
CValueBase::CValueBase(char */*pString*/, int /*nTime_*/, int /*nQulity_*/)
{
    //功能:
    //参数:char * pString
    //参数:int nTime_
    //参数:int nQulity_
    //返回值:
    //权限:public
    //建立日期: 2013-04-08  16:35
    //说明:
    m_pSource = NULL;
    m_pControlTimer = NULL;
    m_nControlFlag = CONTROL_UNDEFINE;
//    m_varValue.setValue(pString);
//	m_quality.SetValue(nQulity_);
    //	m_time.SetTime(nTime_);
}

/*!
 * \brief  功能概述 设置值
 * \param  参数描述 varValue_是值
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
void CValueBase::SetVarValue(const QVariant varValue_)
{
//    qDebug()<<varValue_;
    m_varValue = varValue_;
}

/*!
 * \brief  功能概述 获取值
 * \param  参数描述 无
 * \return 返回值描述 返回值
 * \author zzy
 * \date   2015/5/27
 */
const QVariant CValueBase::GetVarValue()
{
    return m_varValue;
}

/*!
 * \brief  功能概述 采集点调用，解析返回的报文时调用此函数，设置变量是否修改成功及文字描述
 * \param  参数描述 nControlResult_是控制结果(遥控选择、遥控执行、遥控撤销、遥设)，strMessage_是信息
 * \return 返回值描述
 * \author zzy
 * \date   2015/5/27
 */
void CValueBase::SetWriteResult(int nControlResult_, const QString /*strMessage_*/)
{
//    m_strResultMessage = strMessage_;
    m_nControlFlag = (CONTROL)nControlResult_;
}

/*!
 * \brief  功能概述 转发点调用这个函数，等待变量修改完成
 * \param  参数描述 strResultint是等待的结果，nMaxWaitMilliSecend_是最大等待时间
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/27
 */
bool CValueBase::WaitResult(QString strResultint, int nMaxWaitMilliSecend_)
{
    //功能:
    //参数:int nMaxWaitMilliSecend_
    //返回值:bool
    //权限:virtual public
    //建立日期: 2013-03-17  08:08
    //说明:
    bool bReturn = false;
    m_strResultMessage = "";
    m_nControlFlag = CONTROL_UNDEFINE;
    bReturn = QtWaitForMultipleObjects(nMaxWaitMilliSecend_);
    strResultint = m_strResultMessage;
    qDebug()<<"返回"<<bReturn;
    return bReturn;
}

/*!
 * \brief  功能概述 转发点调用这个函数
 * \param  参数描述 nTimeOut_是等待超时的时间
 * \return 返回值描述 返回true
 * \author zzy
 * \date   2015/5/27
 */
bool CValueBase::QtWaitForMultipleObjects(int nTimeOut_)
{
    if (m_pControlTimer == NULL)
    {
        m_pControlTimer = new QTimer;
        connect(m_pControlTimer,SIGNAL(timeout()),this,SLOT(OnTimeOut()));
    }
    m_pControlTimer->start(nTimeOut_);
    while (1)
    {
        if (CONTROL_FAILURE == m_nControlFlag)
        {
            qDebug()<<"执行失败";
            m_strResultMessage = "失败";
            m_pControlTimer->stop();
            return true;
        }else if (CONTROL_SUCCEED == m_nControlFlag)
        {
            qDebug()<<"执行成功";
            m_strResultMessage = "成功";
            m_pControlTimer->stop();
            return true;
        }else if (CONTROL_TIMEOUT == m_nControlFlag)
        {
            qDebug()<<"执行超时";
            m_strResultMessage = "超时";
            m_pControlTimer->stop();
            return true;
        }
        QCoreApplication::processEvents();
        QThread::usleep(1000);
    }
    return false;
}

/*!
 * \brief  功能概述 获取控制之后的状态
 * \param  参数描述 无
 * \return 返回值描述 返回遥控类型
 * \author zzy
 * \date   2015/5/27
 */
int CValueBase::GetnControlFlag()
{
    return (int)m_nControlFlag;
}

/*!
 * \brief  功能概述 终止等待定时器，定时器时间槽函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
void CValueBase::OnTimeOut()
{
    m_pControlTimer->stop();
    if (CONTROL_FAILURE == m_nControlFlag)
    {
        return;
    }else if (CONTROL_SUCCEED == m_nControlFlag)
    {
        return;
    }else
    {
        m_nControlFlag = CONTROL_TIMEOUT;
        return;
    }
}
