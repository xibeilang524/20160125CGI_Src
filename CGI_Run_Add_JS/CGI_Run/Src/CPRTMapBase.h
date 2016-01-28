/*!
 * \file    CPRTMapBase.h
 * \brief   概述 CPRTMapBase类的头文件
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

#ifndef CPRTMAPBASE_H
#define CPRTMAPBASE_H

#include "../PortInclude/CPRTMapI.h"

#include <QMap>

/*!
 * \class  CPRTMapBase
 * \brief  概述 透明转发的map的管理类
 * \author zzy
 * \date   2015/3/18
 * 详细概述
 */
class CPRTMapBase : public CPRTMapI
{
public:
    CPRTMapBase();
    CPRTI *Find(const int nStationAddress);
    const QList<int> FindAllChannelNumber();
    const QList<int> FindAllZTChannelNumber();
    const QVector<int> FindCollectType();
    const QVector<int> FindTransformType();
    CPRTI *FindChannelNumber(const int nChannelNumber_);
    CPRTI *FindZTChannelNumber(const int nZTChannelNumber);
    CPRTI *FindStationAddress(const int nStationAddress_);
    CPRTI *FindChargePileNumber(const QVariant varChargePileNumber_);
    CPRTI* FineChargingPileID(QByteArray baChargingPileID_);
    bool Add(int nKey_,CPRTI *pPRTI_);
public:
    typedef QMap<int,CPRTI*> PRT_MAP;
    PRT_MAP m_mapPRT;///< 透明转发的map
};

#endif // CPRTMAPBASE_H
