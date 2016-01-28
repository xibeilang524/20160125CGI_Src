#ifndef CGI_D_IEC_104_POINTNODE_H
#define CGI_D_IEC_104_POINTNODE_H

#include <QObject>
#include "CGI_D_IEC_104_PointYX.h"
#include "CGI_D_IEC_104_PointYC.h"
#include "CGI_D_IEC_104_PointYM.h"
#include "CGI_D_IEC_104_PointYK.h"
#include "CGI_D_IEC_104_PointYS.h"
#include <QMap>


///<这个类中的所有点的公共地址都一样
class CGI_D_IEC_104_PublicAddress_Node
{
public:
    bool AddPoint(CGI_D_IEC_104_PointBase *pPoint_);
    QMap<int,CGI_D_IEC_104_PointYX *> *Get_YX_PointMap(ASDU101_TYPE nType);
    QMap<int,CGI_D_IEC_104_PointYC *> *Get_YC_PointMap(ASDU101_TYPE nType);
    QMap<int,CGI_D_IEC_104_PointYM *> *Get_YM_PointMap(ASDU101_TYPE nType);
    QMap<int,CGI_D_IEC_104_PointYK *> *Get_YK_PointMap();
    QMap<int,CGI_D_IEC_104_PointYS *> *Get_YS_PointMap();
    QMap<int,CGI_D_IEC_104_PointBase *> *Get_AllPointMap();

private:
    QMap<int,CGI_D_IEC_104_PointYX *> YX_M_SP_NA_1_PointMap;///< 单点信息
    QMap<int,CGI_D_IEC_104_PointYX *> YX_M_DP_NA_1_PointMap;///< 双点信息

    QMap<int,CGI_D_IEC_104_PointYC *> YC_M_ME_NA_1_PointMap;///< 带品质描述的归一化值（M_ME_NA_1）
    QMap<int,CGI_D_IEC_104_PointYC *> YC_M_ME_NB_1_PointMap;///< 带品质描述的称度化值（M_ME_NB_1）
    QMap<int,CGI_D_IEC_104_PointYC *> YC_M_ME_NC_1_PointMap;///< 带品质描述的短浮点数值（M_ME_NC_1）
    QMap<int,CGI_D_IEC_104_PointYC *> YC_M_ME_ND_1_PointMap;///< 不带品质描述的归一化值（M_ME_ND_1）

    QMap<int,CGI_D_IEC_104_PointYM *> YM_M_IT_NA_1_PointMap;///< 累积量

    ///< 本转发程序，不区分遥控、遥调、遥设类型
    QMap<int,CGI_D_IEC_104_PointYK *> YK_PointMap;///< 遥控

    QMap<int,CGI_D_IEC_104_PointYS *> YS_PointMap;///< 遥设

    QMap<int,CGI_D_IEC_104_PointBase *> m_PointMap;///< 该公共地址所有点的MAP
};


class CGI_D_IEC_104_PointNode : public QObject
{
    Q_OBJECT
public:
    explicit CGI_D_IEC_104_PointNode(QObject *parent = 0);
    bool AddPoint(CGI_D_IEC_104_PointBase *pPoint_);
    QMap<int , CGI_D_IEC_104_PublicAddress_Node *> *GetCommandAddress_PointNode();

    QMap<int , CGI_D_IEC_104_PublicAddress_Node *> m_CommandAddressPointNode;///< int 公共地址  按公共地址进行分类


signals:

public slots:

};

#endif // CGI_D_IEC_104_POINTNODE_H
