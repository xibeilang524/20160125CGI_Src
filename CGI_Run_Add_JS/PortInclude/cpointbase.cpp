/*!
 * \file    cpointbase.cpp
 * \brief   概述 CPointBase类的源文件
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

#include "cpointbase.h"
#include "../PortInclude/CValueI.h"
#include <QString>
#include "../PortInclude/cvaluebase.h"
#include "../PortInclude/CTagI.h"
#include "../PortInclude/cprotocolbase.h"
#include <QDebug>

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
CPointBase::CPointBase()
{
    m_pProtocol = NULL;     //变量所在的通道
    m_pTag= NULL;
    m_pValue = new CValueBase;
}

/*!
 * \brief  功能概述 析构函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
CPointBase::~CPointBase()
{
    if (m_pValue != NULL)
    {
        delete m_pValue;
        m_pValue = NULL;
    }
}

/*!
 * \brief  功能概述 处理当值发生变化时，处理遥控、遥设变量，此函数由转发驱动调用
 * \param  参数描述 valueSet_是当前值
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/27
 */
bool CPointBase::OnValueChange(QVariant valueSet_)
{
    Q_ASSERT(m_pValue != NULL);
    return m_pProtocol->OnTagChange(this,valueSet_);
}

/*!
 * \brief  功能概述 初始化函数，子类要初始化变量值时使用
 * \param  参数描述 strConfig是配置
 * \return 返回值描述 返回true
 * \author zzy
 * \date   2015/5/27
 */
bool CPointBase::OnCreate(const QString strConfig)
{
    m_strConfig = strConfig;
//	CParameterXML para(m_strConfig);
//	m_strPointName = para.GetString("Name","");
//	m_strPointComment = para.GetString("Desc","");
//	m_strRTDBName = para.GetString("Link","");

    return true;
}

/*!
 * \brief  功能概述 获取QString类型的配置
 * \param  参数描述 strAttribName是传入参数
 * \return 返回值描述 返回QString类型的配置
 * \author zzy
 * \date   2015/5/27
 */
QString CPointBase::GetConfigString(const QString /*strAttribName*/)
{
    //功能:
    //参数:const string & strAttribName
    //返回值:string
    //权限:public
    //建立日期: 2013-03-18  20:13
    //说明:
//	CParameterXML para(m_strConfig);
//	return  para.GetString(strAttribName.c_str(),"");
    return "";
}

/*!
 * \brief  功能概述 获取int类型的配置
 * \param  参数描述 strAttribName是传入参数
 * \return 返回值描述 返回int类型的配置
 * \author zzy
 * \date   2015/5/27
 */
int CPointBase::GetConfigInt(const QString /*strAttribName*/)
{
    //功能:
    //参数:const string & strAttribName
    //返回值:int
    //权限:public
    //建立日期: 2013-03-18  20:23
    //说明:
//	CParameterXML para(m_strConfig);
//	return  para.GetInt(strAttribName.c_str(),0);
    return 0;
}

/*!
 * \brief  功能概述 获取double类型的配置
 * \param  参数描述 strAttribName是传入参数
 * \return 返回值描述 返回double类型的配置
 * \author zzy
 * \date   2015/5/27
 */
double CPointBase::GetConfigDouble(const QString /*strAttribName*/)
{
    //功能:
    //参数:const string & strAttribName
    //返回值:double
    //权限:public
    //建立日期: 2013-03-18  20:23
    //说明:
//	CParameterXML para(m_strConfig);
//	return  para.GetFloat(strAttribName.c_str(),0);
    return 0;
}

/*!
 * \brief  功能概述
 * \param  参数描述
 * \return 返回值描述
 * \author zzy
 * \date   2015/5/27
 */
void CPointBase::GetBinary(BYTE */*pBuffer*/, int /*nBufferSize*/)
{
    //功能:
    //参数:BYTE * pBuffer
    //参数:int & nBufferSize
    //返回值:void
    //权限:virtual public
    //建立日期: 2013-03-15  15:24
    //说明:
    Q_ASSERT(false);
}

/*!
 * \brief  功能概述
 * \param  参数描述
 * \return 返回值描述
 * \author zzy
 * \date   2015/5/27
 */
void CPointBase::SetBinary(BYTE */*pBuffer*/, int /*nBufferSize*/, int /*nTime_*/, int /*nQulity_*/)
{
    //功能:
    //参数:BYTE * pBuffer
    //参数:int nBufferSize
    //返回值:void
    //权限:virtual public
    //建立日期: 2013-03-15  15:24
    //说明:
    Q_ASSERT(false);
}

/*!
 * \brief  功能概述 获取该点中值的指针
 * \param  参数描述 无
 * \return 返回值描述 返回该点中值的指针
 * \author zzy
 * \date   2015/5/27
 */
CValueI *CPointBase::GetValue()
{
    //功能:
    //参数:IValue & valueReturn_
    //返回值:void
    //权限:virtual public
    //建立日期: 2013-03-15  15:24
    //说明:
    Q_ASSERT(m_pValue != NULL);
    return m_pValue;
}

int CPointBase::GetDeviceAddress()
{
    return 0;
}
/*!
 * \brief  功能概述 获取该点的名称
 * \param  参数描述 无
 * \return 返回值描述 返回该点的名称
 * \author zzy
 * \date   2015/5/27
 */
QString CPointBase::GetPointName()
{
    return m_strPointName;
}

/*!
 * \brief  功能概述 设置该点的名称
 * \param  参数描述 strTagName是点的名称
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
void CPointBase::SetPointName(const QString strTagName_)
{
    m_strPointName = strTagName_;
}

/*!
 * \brief  功能概述 获取点的注释
 * \param  参数描述 无
 * \return 返回值描述 返回点的注释
 * \author zzy
 * \date   2015/5/27
 */
QString CPointBase::GetPointComment()
{
    return m_strPointComment;
}

/*!
 * \brief  功能概述 设置点的注释
 * \param  参数描述 strTagComment_是点的注释
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
void CPointBase::SetPointComment(const QString strTagComment_)
{
    m_strPointComment = strTagComment_;
}

CTagI *CPointBase::GetTag()
{
    return m_pTag;
}

/*!
 * \brief  功能概述
 * \param  参数描述
 * \return 返回值描述
 * \author zzy
 * \date   2015/5/27
 */
void CPointBase::ConnectTag(CTagI *pTag_)
{
    //功能:
    //参数:ITag * pTag
    //返回值:void
    //权限:virtual public
    //建立日期: 2013-03-17  07:39
    //说明:
    Q_ASSERT(pTag_ != NULL);
    if(pTag_ == NULL)
        return ;

     m_pTag = pTag_;
     int nNotify = GetConfigInt("Notify");
     if(nNotify == 0)
     {
         pTag_->SetCollection(this);
     }
     else if(nNotify == 1)
     {
         pTag_->SetNotify(this);
     }
     else if(nNotify ==2)
     {
         pTag_->SetCollection(this);
         pTag_->SetNotify(this);

     }
}

/*!
 * \brief  功能概述
 * \param  参数描述
 * \return 返回值描述
 * \author zzy
 * \date   2015/5/27
 */
void CPointBase::DisConnectTag()
{
    //功能:
    //返回值:void
    //权限:virtual public
    //建立日期: 2013-03-17  07:39
    //说明:
    Q_ASSERT(m_pTag != NULL);
    if(m_pTag == NULL)
        return ;

    int nNotify = GetConfigInt("Notify");
    if(nNotify == 0)
    {
        m_pTag->DisCollection(this);
    }
    else if(nNotify == 1)
    {
        m_pTag->DisNotify(this);
    }
    else if(nNotify ==2)
    {
        m_pTag->DisCollection(this);
        m_pTag->DisNotify(this);
    }
}

/*!
 * \brief  功能概述 获取点的类型
 * \param  参数描述 无
 * \return 返回值描述 返回点的类型
 * \author zzy
 * \date   2015/5/27
 */
int CPointBase::GetPointType()
{
    return m_nTagType;
}

/*!
 * \brief  功能概述 设置点的类型
 * \param  参数描述 nType是点的类型
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
void CPointBase::SetPointtype(int nType_)
{
    m_nTagType = nType_;
}

/*!
 * \brief  功能概述 获取驱动类的指针
 * \param  参数描述 无
 * \return 返回值描述 返回驱动类的指针
 * \author zzy
 * \date   2015/5/27
 */
CProtocolI *CPointBase::GetProtocol()
{
    return m_pProtocol;
}

/*!
 * \brief  功能概述 设置驱动类的指针
 * \param  参数描述 pProtocol_是驱动类的指针
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/27
 */
void CPointBase::SetProtocol(CProtocolI *pProtocol_)
{
    m_pProtocol = pProtocol_;
}
