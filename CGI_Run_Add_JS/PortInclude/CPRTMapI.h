/*!
 * \file    CPRTMapI.h
 * \brief   概述 CPRTMapI类的头文件
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

#ifndef CPRTMAPI_H
#define CPRTMAPI_H
class CPRTI;
class QVariant;
#include <QVector>

/*!
 * \class  CPRTMapI
 * \brief  概述 通道管理类
 * \author zzy
 * \date   2015/5/27
 * 详细概述
 */
class CPRTMapI
{
public:
    ///查找函数
    virtual CPRTI *Find(const int nStationAddress_) = 0;
    virtual const QVector<int> FindCollectType() = 0;
    virtual const QVector<int> FindTransformType() = 0;
    virtual const QList<int> FindAllChannelNumber() = 0;
    virtual const QList<int> FindAllZTChannelNumber() = 0;
    virtual CPRTI *FindChannelNumber(const int nChannelNumber_) = 0;
    virtual CPRTI *FindZTChannelNumber(const int nZTChannelNumber) = 0;
    virtual CPRTI *FindStationAddress(const int nStationAddress_) = 0;
    virtual CPRTI *FindChargePileNumber(const QVariant varChargePileNumber_) = 0;
    virtual bool  Add(int nKey_,CPRTI *pPRTI_) = 0;
    virtual CPRTI* FineChargingPileID(QByteArray strChargingPileID_) = 0;
};
#endif // CPRTMAPI_H
