/*!
 * \file    CTagBase.cpp
 * \brief   概述 CTagBase类的源文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/5/22
 *
 * \b       修改记录：
 * \li      2015/5/22
 *  添加注释
 */

#include "ctagbase.h"
#include "../PortInclude/CValueI.h"
#include "../PortInclude/CPointI.h"
#include <QString>
#include <QDebug>
#include <QFile>
#include <QTime>
#include <QTextStream>
#include <assert.h>
#include "Src/cmyfunction.h"
#include "../PortInclude/CRTDBSharedMemoryLibI.h"
#include "CRTDBSharedMemoryThread.h"
//extern QFile DebugFile;
//extern QTextStream out;
void DebugMessageFile(QString /*strDebug*/)
{

//    out.setFieldWidth(15);
//    out.setFieldAlignment(QTextStream::AlignLeft);
//    out<<strDebug<<"\r\n";
//    DebugFile.flush();
}

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/22
 */
CTagBase::CTagBase()
{
    m_strName = "";
    m_strDesc = "";
    m_nID = -1;
    m_nProjectDataType = DATA_TYPE_UNDEFINE;///< 变量工程值类型 该值类型是取自实时数据库中
    m_pProjectValue = NULL; ///< 变量工程值 用于界面显示 做系数和基数处理 (原始值+基数)*系数
    m_pPointCollection = NULL;
}

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 nType_是值类型，strName_是点的名字
 * \return 返回值描述
 * \author zzy
 * \date   2015/5/22
 */
CTagBase::CTagBase(int nType_,QString strName_, QString strDesc_, int nID_, DATA_TYPE nDataProjectType_, CValueI *pValue_)
{
    m_nType = nType_;
    m_strName = strName_;
    m_strDesc = strDesc_;
    m_nID = nID_;
    m_nProjectDataType = nDataProjectType_;// 变量工程值类型 该值类型是取自实时数据库中
    m_pProjectValue = pValue_; //变量工程值 用于界面显示 做系数和基数处理 (原始值+基数)*系数
    m_pPointCollection = NULL;
}

/*!
 * \brief  功能概述 析构函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/22
 */
CTagBase::~CTagBase()
{
    if(m_pProjectValue != NULL)
    {
        delete m_pProjectValue;
    }
    QList<CPointI *>::iterator it = m_PointDelivers.begin();
    for(;it!=m_PointDelivers.end();++it)
    {
        m_PointDelivers.erase(it);
        delete (*it);
    }
}

/*!
 * \brief  功能概述 获取点的名字
 * \param  参数描述 无
 * \return 返回值描述 返回点的名字
 * \author zzy
 * \date   2015/5/22
 */
QString CTagBase::GetName()
{
    return m_strName;
}

/*!
 * \brief  功能概述 获取点的描述
 * \param  参数描述 无
 * \return 返回值描述 返回点的描述
 * \author zzy
 * \date   2015/5/22
 */
QString CTagBase::GetDesc()
{
    return m_strDesc;
}

/*!
 * \brief  功能概述 获取点的ID号
 * \param  参数描述 无
 * \return 返回值描述 返回点的ID号
 * \author zzy
 * \date   2015/5/22
 */
int CTagBase::GetTagID()
{
    return m_nID;
}

/*!
 * \brief  功能概述 设置点的ID号
 * \param  参数描述 ID_是点的ID号
 * \return 返回值描述 返回点的ID号
 * \author zzy
 * \date   2015/5/22
 */
int CTagBase::SetTagID(int ID_)
{
    m_nID = ID_;
    return m_nID;
}

/*!
* \brief  功能概述 获取点的类型 YX YC YM YK YS
* \param  参数描述 无
* \return 返回值描述 返回点的类型
* \author zzy
* \date   2015/3/24
*/
int CTagBase::GetType()
{    
    return m_nType;
}

//void CTagBase::SetOriginalDataType(DATA_TYPE nDataType_)
//{
//    m_nOriginalDataType = nDataType_;
//}

//DATA_TYPE CTagBase::GetOriginalDataType()
//{
//    return m_nOriginalDataType;
//}

//bool CTagBase::SetOriginalValue(CValueI *pValue_)
//{
//    Q_ASSERT(pValue_ != NULL);
//    if(pValue_ != NULL)
//    {
//        if(m_pOriginalValue != NULL)
//        {
//            delete m_pOriginalValue;
//        }
//        m_pOriginalValue = pValue_;
//    }
//    else
//    {
//        return false;
//    }

//    for(int i=0;i<m_PointDelivers.count();i++)
//    {
//        m_PointDelivers.at(i)->OnValueChange(*pValue_);
//    }
//    return true;
//}

//CValueI *CTagBase::GetOriginalValue()
//{
//    return m_pOriginalValue;
//}

/*!
 * \brief  功能概述 设置点的值类型
 * \param  参数描述 nDataType_值类型
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/22
 */
void CTagBase::SetProjectDataType(DATA_TYPE nDataType_)
{
    assert(isDataType(nDataType_));
//    Q_ASSERT_X(isDataType(nDataType_), "void CTagBase::SetProjectDataType(DATA_TYPE nDataType_)", "变量的值类型错误");
    m_nProjectDataType = nDataType_;
}

/*!
 * \brief  功能概述 获取工程值类型
 * \param  参数描述 无
 * \return 返回值描述 返回工程值类型
 * \author zzy
 * \date   2015/5/22
 */
DATA_TYPE CTagBase::GetProjectDataType()
{
    return m_nProjectDataType;
}

/*!
 * \brief  功能概述 获取工程值的指针
 * \param  参数描述 无
 * \return 返回值描述 返回工程值的指针
 * \author zzy
 * \date   2015/5/22
 */
CValueI *CTagBase::GetProjectValue()
{
    return m_pProjectValue;
}

/*!
 * \brief  功能概述 设置采集点的指针
 * \param  参数描述 pPoint_是采集点的指针
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/22
 */
void CTagBase::SetCollection(CPointI *pPoint_)
{
    assert(pPoint_ != NULL);
    if(pPoint_ != NULL)
    {
        m_pPointCollection = pPoint_;
    }
}

/*!
 * \brief  功能概述 删除采集点指针
 * \param  参数描述 pPoint_是采集点指针
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/22
 */
void CTagBase::DisCollection(CPointI *pPoint_)
{
    if(m_pPointCollection == pPoint_)
    {
        delete m_pPointCollection;
        m_pPointCollection = NULL;
    }
}

/*!
 * \brief  功能概述 添加转发点指针
 * \param  参数描述 pPoint_是转发点指针
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/22
 */
void CTagBase::SetNotify(CPointI *pPoint_)
{
    assert(pPoint_ != NULL);
    m_PointDelivers.push_back(pPoint_);
}

/*!
 * \brief  功能概述 删除转发点指针
 * \param  参数描述 pPoint_是要删除的转发点的指针
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/22
 */
void CTagBase::DisNotify(CPointI *pPoint_)
{
    assert(pPoint_ != NULL);
    QList<CPointI *>::iterator it = m_PointDelivers.begin();
    for(;it!=m_PointDelivers.end();++it)
    {
        if((*it) == pPoint_)
        {
            m_PointDelivers.erase(it);
            delete (*it);
            break;//如果不要，可以删除的更彻底
        }
    }
}

/*!
 * \brief  功能概述 获取采集点指针
 * \param  参数描述 无
 * \return 返回值描述 返回采集点指针
 * \author zzy
 * \date   2015/5/22
 */
CPointI *CTagBase::GetPointCollection()
{
    return m_pPointCollection;
}

/*!
 * \brief  功能概述 设置值
 * \param  参数描述 VarProject_是工程值，VarOriginal_是原始值
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/22
 */
void CTagBase::SetValue(CPointI *pPoint_, QVariant VarProject_, QVariant VarOriginal_)
{
    qDebug()<<this->GetName()<<"工程值: "<<VarProject_<<"  "<<"原始值: "<<VarOriginal_;
    if (m_pProjectValue != NULL)
    {
        m_pProjectValue->SetVarValue(VarProject_);
    }
    if (NULL != m_pPointCollection)
    {
        if (pPoint_ != m_pPointCollection)
        {
            m_pPointCollection->OnValueChange(VarProject_);
        }
        m_pPointCollection->GetValue()->SetVarValue(VarProject_);
    }

    CMyFunction::m_pRTDBSharedMemoryThread->SetValue(this->m_nID,VarProject_);
    for(int i=0;i<m_PointDelivers.count();i++)
    {
        if (m_PointDelivers.at(i) != pPoint_)
        {
            m_PointDelivers.at(i)->OnValueChange(VarProject_);///通知所有转发点 值发生了变化
        }
    }
}


/*!
 * \brief  功能概述 是否为正确的值类型
 * \param  参数描述 nDataType_值类型
 * \return 返回值描述 正确返回true，错误返回false
 * \author zzy
 * \date   2015/5/22
 */
bool CTagBase::isDataType(int nDataType_)
{
    bool isTrue;
    isTrue =   (DATA_TYPE_UNDEFINE == nDataType_)
            || (DATA_TYPE_BOOLEAN == nDataType_)
            || (DATA_TYPE_INT8U == nDataType_)
            || (DATA_TYPE_INT8S == nDataType_)
            || (DATA_TYPE_INT16U == nDataType_)
            || (DATA_TYPE_INT16S == nDataType_)
            || (DATA_TYPE_INT32U == nDataType_)
            || (DATA_TYPE_INT32S == nDataType_)
            || (DATA_TYPE_INT64U == nDataType_)
            || (DATA_TYPE_INT64S == nDataType_)
            || (DATA_TYPE_INT128U == nDataType_)
            || (DATA_TYPE_INT128S == nDataType_)
            || (DATA_TYPE_FP32 == nDataType_)
            || (DATA_TYPE_FP64 == nDataType_)
            || (DATA_TYPE_STRING == nDataType_)
            || (DATA_TYPE_UNICODE == nDataType_)
            || (DATA_TYPE_BINARY == nDataType_);
    return isTrue;
}


