/*!
 * \file    CMyFunction.h
 * \brief   概述 CMyFunction类的头文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/1/16
 *
 * \b       修改记录：
 * \li      2015/1/16
 *  添加注释
 */

#ifndef CMYFUNCTION_H
#define CMYFUNCTION_H

#include <QList>
#include <QString>
#include <QFile>
class CProtocolI;

/*!
 * \enum 枚举名 TREEITEMTYPE
 * \brief 概述 树节点的类型
 * \author zzy
 * \date 2015/1/8
 * 详细概述
 */
enum TREEITEMTYPE{
    /// 树节点类型                类型描述         节点显示             窗口              右键是否有效
/// 空白窗口
    TreeItemTYPE_VoidItem = -1, //空节点         ->自定义显示         ->空白窗口         ->右键无效
    TreeItemTYPE_ProjectName,   //项目名称        ->仅仅显示工程名称    ->空白窗口         ->右键无效
    TreeItemTYPT_RealLibItem,   //实时库节点      ->显示"实时库"       ->空白窗口         ->右键弹出新建实时库文件菜单
    TreeItemTYPE_NetItem,       //网络节点        ->仅仅显示"网络节点"  ->空白窗口         ->右键新建"服务器%d"
    TreeItemTYPE_ColChannel,    //采集通道节点    ->显示"采集通道"      ->空白窗口         ->右键新建采集通道
    TreeItemTYPE_TraChannel,    //转发通道节点     ->显示"转发通道"     ->空白窗口         ->右键新建转发通道
    TreeItemTYPE_Windows,       //窗口
    TreeItemTYPE_Save,          //存储
/// 只有属性
    TreeItemTYPE_RealLibName,   //实时库文件名节点 ->显示某场站         ->属性窗口         ->右键弹出新建管理单元和删除该实时库菜单
    TreeItemTYPE_NetItemChild,  //网络子节点      ->显示网络子节点      ->属性窗口         ->右键无效
    TreeItemTYPE_ColChannelChild,//采集通道       ->采集通道           ->属性窗口         ->右键新建采集设备和删除该采集通道
    TreeItemTYPE_TraChannelChild,//转发通道       ->转发通道           ->属性窗口         ->右键新建转发设备和删除该转发通道
/// 属性和5种表格
    TreeItemTYPE_SwitchItem,    //回路节点        ->显示管理单元       ->属性窗口和5种表格 ->右键新建管理单元和删除该管理单元
    TreeItemTYPE_ColDeviceItem, //采集设备节点     ->显示设备编号       ->属性窗口和5种表格 ->右键删除该设备节点
    TreeItemTYPE_TraDeviceItem, //转发设备节点     ->显示设备编号       ->属性窗口和5种表格 ->右键删除该设备节点
    TreeItemTYPE_DatebaseItem,  //数据库节点周期存储 ->显示显示数据库名字  ->属性窗口和一个表格 ->右键删除该设备节点
    TreeItemTYPE_DatabaseChangeItem,//数据库节点,变化存储
};

class CMyTextBrowser;

/*!
 * \struct DisplayFrame
 * \brief  概述 报文结构体
 * \author zzy
 * \date   2015/1/16
 * 详细概述
 */
struct DisplayFrame
{
    int m_nType;
    QString m_strFrame;
    int m_nChannelNumber;
};

class CDatabaseThread;
class DatabaseMessage{
public:
    CDatabaseThread *m_pDatabaseThread;
    QString m_strDatabaseFileName;
    QString m_strDatabaseName;
    QString m_strProtocolFileName;
};
class CRTDBSharedMemoryThread;
/*!
 * \class  CMyFunction
 * \brief  概述 使用静态函数替代,使用静态变量替代全局变量,
 * \author zzy
 * \date   2015/1/16
 * 详细概述
 */
class CMyFunction
{
public:
    CMyFunction();
    static int ChannelNumber;///< 当前窗口号 当前通道号
    static QList<DisplayFrame> DisplayFrameList;
    static bool IsShowTextBrowserWindow;
    static int TimeOut;///< 写入数据库的周期
    static QString strProjectName;
    static QString MonitorIPAddress;
    static CProtocolI *pMonitorProtocol;
    static QFile m_sFile;///< 运行日志文件
    static CRTDBSharedMemoryThread *m_pRTDBSharedMemoryThread;
};

#endif // CMYFUNCTION_H
