/*!
 * \file
 * \brief   概述
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/3/24
 *
 * \b       修改记录：
 * \li      2015/3/24
 *  添加注释
 */

#include "cpointbase.h"
#include "../PortInclude/CValueI.h"
#include <QString>
#include "../PortInclude/cvaluebase.h"
#include "../PortInclude/CTagI.h"
#include <QDebug>
#include "CProtocolDeliverPRT_IEC_104.h"
//#include "cprotocoldeliveriec_104.h"
CPointBase::CPointBase()
{
//    qDebug()<<"a";
    m_pProtocol = NULL;     //变量所在的通道
//    m_pDevice= NULL;         //变量所在的结点
    m_pTag= NULL;
    m_pValue = new CValueBase;
}

CPointBase::~CPointBase()
{
    if (m_pValue != NULL)
    {
        delete m_pValue;
        m_pValue = NULL;
    }
}
//ADD  ZHANG  此函数由转发驱动调用
bool CPointBase::OnValueChange(QVariant valueSet_)
{
//    功能:处理当值发生变化时
//    功能:处理遥控、遥设变量
//    qDebug()<<"转发";
    Q_ASSERT(m_pValue != NULL);
    return m_pProtocol->OnTagChange(this,valueSet_);
}

bool CPointBase::OnCreate(const QString strConfig)
{
    //功能:
    //参数:const string & strConfig
    //返回值:CPointBase
    //权限:virtual public
    //建立日期: 2013-03-17  07:44
    //说明:  子类要初始化变量值时使用
    m_strConfig = strConfig;
//	CParameterXML para(m_strConfig);
//	m_strPointName = para.GetString("Name","");
//	m_strPointComment = para.GetString("Desc","");
//	m_strRTDBName = para.GetString("Link","");

    return true;
}

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
    return ";";
}

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


QString CPointBase::GetPointName()
{
    return m_strPointName;
}

void CPointBase::SetPointName(const QString strTagName)
{
    m_strPointName = strTagName;
}

QString CPointBase::GetPointComment()
{
    return m_strPointComment;
}

void CPointBase::SetPointComment(const QString strTagComment)
{
    m_strPointComment = strTagComment;
}

CTagI *CPointBase::GetTag()
{
    return m_pTag;
}

void CPointBase::ConnectTag(CTagI *pTag)
{
    //功能:
    //参数:ITag * pTag
    //返回值:void
    //权限:virtual public
    //建立日期: 2013-03-17  07:39
    //说明:
    Q_ASSERT(pTag != NULL);
    if(pTag == NULL)
        return ;

     m_pTag = pTag;
     int nNotify = GetConfigInt("Notify");
     if(nNotify == 0)
     {
         pTag->SetCollection(this);
     }
     else if(nNotify == 1)
     {
         pTag->SetNotify(this);
     }
     else if(nNotify ==2)
     {
         pTag->SetCollection(this);
         pTag->SetNotify(this);

     }
}

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

int CPointBase::GetPointType()
{
    return m_nTagType;
}

void CPointBase::SetPointtype(int nType)
{
    m_nTagType = nType;
}

CProtocolI *CPointBase::GetProtocol()
{
    return m_pProtocol;
}

void CPointBase::SetProtocol(CProtocolI *pProtocol)
{
    m_pProtocol = pProtocol;
}
