/*!
 * \file    CTagBase.h
 * \brief   概述 CTagBase类的头文件
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

#ifndef CTAGBASE_H
#define CTAGBASE_H

#include "../PortInclude/CTagI.h"
#include <QString>
#include <QList>
#include <QVariant>

extern "C" void DebugMessageFile(QString strDebug);

/*!
 * \class  CTagBase
 * \brief  概述 变量类
 * \author zzy
 * \date   2015/5/22
 * 详细概述
 */
class CTagBase : public CTagI
{
public:
    CTagBase();
    CTagBase(int nType_,QString strName_, QString strDesc_, int nID_, DATA_TYPE nDataProjectType_, CValueI *pValue_);
    ~CTagBase();

public:
    virtual QString GetName();  //获取点名称
    virtual QString GetDesc() ; //点描述
    virtual int GetTagID();     //获取点id
    virtual int SetTagID(int ID_);
    int GetType();
//! 工程值处理函数
    virtual void SetProjectDataType(DATA_TYPE nDataType_);//设置工程值类型 该值类型是取自实时数据库中
    virtual DATA_TYPE GetProjectDataType();//变量工程值类型 该值类型是取自实时数据库中
    virtual CValueI *GetProjectValue();//获取工程值
//! 采集点的设置
    virtual void SetCollection(CPointI *pPoint_);//设置采集量
//! 转发点的设置
    virtual void SetNotify(CPointI *pPoint_);//设置转发

//! 声明为保护类型是为了防止调用  目前不需要调用以下两个函数
//protected:
    virtual void DisCollection(CPointI *pPoint_);//取消采集
    virtual void DisNotify(CPointI *pPoint_);//取消转发

    virtual CPointI *GetPointCollection();//获取采集点信息

    virtual void SetValue(CPointI* pPoint_,QVariant VarProject_, QVariant VarOriginal_);
    bool isDataType(int nDataType_);
//Data Member
private:
    QString m_strName;
    QString m_strDesc;
    int m_nID;
    int m_nType;///<zzy 2015/3/24 修改:点的类型 YX YC YM YK YS

    DATA_TYPE m_nProjectDataType;///< 变量工程值类型 该值类型是取自实时数据库中
    CValueI *m_pProjectValue; ///< 变量工程值 用于界面显示 做系数和基数处理 (原始值+基数)*系数

    CPointI *m_pPointCollection;///<采集点
    QList<CPointI *> m_PointDelivers;///<转发点
};

#endif // CTAGBASE_H
