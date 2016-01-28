/*!
 * \file    CPointBase.h
 * \brief   概述 CPointBase类的头文件
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

#ifndef CPOINTBASE_H
#define CPOINTBASE_H

#include "../PortInclude/CPointI.h"
#include "../PortInclude/CTagI.h"

class CDatabaseProtocolI;

/*!
 * \class  CPointBase
 * \brief  概述 数据库点的参数类
 * \author zzy
 * \date   2015/5/27
 * 详细概述
 */
class CPointBase : public CPointI
{
public:

    int m_nPointIndex;///< 索引号
    int m_nPeriodicStorage;///< 存储周期
    int m_nStorageMode;///< 存储模式
    int m_nPointType;

    CPointBase();
    virtual bool OnValueChange(QVariant valueSet_); ///< 响应变量什发生变化
    virtual CValueI *GetValue();    ///< 获取值
    virtual int GetDeviceAddress(); ///< 获取设备地址
    CDatabaseProtocolI *m_pProtocol;
    CValueI       *m_pValue;    ///< 变量的值
    QString m_strRTDBName;      ///< 关联的db变量名
    CTagI         *m_pTag;      ///< 变量所在tag
};

#endif // CPOINTBASE_H
