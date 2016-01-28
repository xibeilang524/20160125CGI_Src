/*!
 * \file    CPRTMapBase.cpp
 * \brief   概述 CPRTMapBase类的源文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/3/18
 *
 * \b       修改记录：
 * \li      2015/3/18
 *  添加注释
 */

#include "CPRTMapBase.h"
#include "../PortInclude/CPRTI.h"
#include "../PortInclude/cprotocoli.h"
#include <QDebug>

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/3/18
 */
CPRTMapBase::CPRTMapBase()
{

}

/*!
 * \brief  功能概述 按照站地址查找map中的value
 * \param  参数描述 nStationAddress_站地址
 * \return 返回值描述 查找成功返回map中相应的value，失败返回NULL
 * \author zzy
 * \date   2015/3/18
 */
CPRTI *CPRTMapBase::Find(const int nStationAddress)
{
    QMap<int, CPRTI*>::iterator i;
    for (i = m_mapPRT.begin(); i != m_mapPRT.end(); ++i)
    {
        if (i.value()->GetnStationAddress() == nStationAddress)
        {
            CPRTI *pPRTI = i.value();
            return pPRTI;
        }
    }
    return NULL;
}

/*!
 * \brief  功能概述 查找所有的通道号
 * \param  参数描述 无
 * \return 返回值描述 返回所有通道号的list
 * \author zzy
 * \date   2015/3/18
 */
const QList<int> CPRTMapBase::FindAllChannelNumber()
{
    QMap<int, CPRTI*>::iterator i;
    QList<int> result;
    for (i = m_mapPRT.begin(); i != m_mapPRT.end(); ++i)
    {
        int nChannelNumber = i.key();
        result.append(nChannelNumber);
    }
    return result;
}

/*!
 * \brief  功能概述 查找所有组态的通道号
 * \param  参数描述 无
 * \return 返回值描述 返回查找到的组态号组成的list
 * \author zzy
 * \date   2015/3/18
 */
const QList<int> CPRTMapBase::FindAllZTChannelNumber()
{
    QMap<int, CPRTI*>::iterator i;
    QList<int> result;
    for (i = m_mapPRT.begin(); i != m_mapPRT.end(); ++i)
    {
        int nZTChannelNumber = i.value()->GetProcotolI()->GetZTChannelNumber();
        result.append(nZTChannelNumber);
    }
    return result;
}

/*!
 * \brief  功能概述 查找所有采集类型的通道号
 * \param  参数描述 无
 * \return 返回值描述 返回所有采集类型的通道号组成的QVector数组
 * \author zzy
 * \date   2015/3/18
 */
const QVector<int> CPRTMapBase::FindCollectType()
{
    QMap<int, CPRTI*>::iterator i;
    QVector<int> result;
    for (i = m_mapPRT.begin(); i != m_mapPRT.end(); ++i)
    {
        if (i.value()->GetDriverType() == Collect_Type)
        {
            int key = i.key();
            result.append(key);
        }
    }
    return result;
}

/*!
 * \brief  功能概述 查找所有转发类型的通道号
 * \param  参数描述 无
 * \return 返回值描述 返回所有转发类型的通道的通道号组成的QVector数组
 * \author zzy
 * \date   2015/3/18
 */
const QVector<int> CPRTMapBase::FindTransformType()
{
    QMap<int, CPRTI*>::iterator i;
    QVector<int> result;
    for (i = m_mapPRT.begin(); i != m_mapPRT.end(); ++i)
    {
//        qDebug()<<i.key();
        if (i.value()->GetDriverType() == Transform_Type)
        {
            int key = i.key();
            result.append(key);
        }
    }
    return result;
}

/*!
 * \brief  功能概述 按照系统分配的通道号查找通道的对象指针
 * \param  参数描述 nChannelNumber_是查找的系统分配的通道号
 * \return 返回值描述 如果查找到则返回查找到的通道的对象指针，否则返回NULL
 * \author zzy
 * \date   2015/3/19
 */
CPRTI *CPRTMapBase::FindChannelNumber(const int nChannelNumber_)
{
    QMap<int, CPRTI*>::iterator i;
    for (i = m_mapPRT.begin(); i != m_mapPRT.end(); ++i)
    {
//        qDebug()<<"111111:"<<i.key()<<nChannelNumber_;
        if (i.key() == nChannelNumber_)
        {
            CPRTI *pPRTI = i.value();
            return pPRTI;
        }
    }
    return NULL;
}

/*!
 * \brief  功能概述 按照组态系统组态的通道号查找通道的对象指针
 * \param  参数描述 nZTChannelNumber是查找的组态时通道的通道号
 * \return 返回值描述 如果查找到则返回查找到的通道的对象指针，否则返回NULL
 * \author zzy
 * \date   2015/5/22
 */
CPRTI *CPRTMapBase::FindZTChannelNumber(const int nZTChannelNumber)
{
    QMap<int, CPRTI*>::iterator i;
    for (i = m_mapPRT.begin(); i != m_mapPRT.end(); ++i)
    {
//        qDebug()<<"111111:"<<i.key()<<nChannelNumber_;
        if (i.value()->GetProcotolI()->GetZTChannelNumber() == nZTChannelNumber)
        {
            CPRTI *pPRTI = i.value();
            return pPRTI;
        }
    }
    return NULL;
}

/*!
 * \brief  功能概述 按照站地址查找通道的对象指针
 * \param  参数描述 nStationAddress_是站地址
 * \return 返回值描述 如果查找到则返回查找到的通道的对象指针，否则返回NULL
 * \author zzy
 * \date   2015/3/19
 */
CPRTI *CPRTMapBase::FindStationAddress(const int nStationAddress_)
{
    QMap<int, CPRTI*>::iterator i;
    for (i = m_mapPRT.begin(); i != m_mapPRT.end(); ++i)
    {
        if (i.value()->GetnStationAddress() == nStationAddress_)
        {
            CPRTI *pPRTI = i.value();
            return pPRTI;
        }
    }
    return NULL;
}

/*!
 * \brief  功能概述
 * \param  参数描述
 * \return 返回值描述
 * \author zzy
 * \date   2015/3/19
 */
CPRTI *CPRTMapBase::FindChargePileNumber(const QVariant varChargePileNumber_)
{
    QMap<int, CPRTI*>::iterator i;
    for (i = m_mapPRT.begin(); i != m_mapPRT.end(); ++i)
    {
        if (i.value()->GetChargePileNumber() == varChargePileNumber_)
        {
            CPRTI *pPRTI = i.value();
            return pPRTI;
        }
    }
    return NULL;
}

CPRTI *CPRTMapBase::FineChargingPileID(QByteArray baChargingPileID_)
{
    QMap<int, CPRTI*>::iterator i;
    for (i = m_mapPRT.begin(); i != m_mapPRT.end(); ++i)
    {
        if (i.value()->GetDriverType() == Collect_Type)
        {
            if (i.value()->GetChargingPileID() == baChargingPileID_)
            {
                CPRTI *pPRTI = i.value();
                return pPRTI;
            }
        }
    }
    return NULL;
}

/*!
 * \brief  功能概述 向通道map中添加通道对象
 * \param  参数描述 nKey_是键值，系统分配的通道号，pPRTI_是通道的对象指针
 * \return 返回值描述 若添加成功返回true，添加失败返回false
 * \author zzy
 * \date   2015/3/18
 */
bool CPRTMapBase::Add(int nKey_, CPRTI *pPRTI_)
{
    if(!m_mapPRT.contains(nKey_))
    {
        m_mapPRT.insert(nKey_, pPRTI_);
        return true;
    }
    return false;
}
