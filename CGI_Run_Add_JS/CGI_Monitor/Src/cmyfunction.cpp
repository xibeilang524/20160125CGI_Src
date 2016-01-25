/*!
 * \file    CMyFunction.cpp
 * \brief   概述 CMyFunction类的源文件
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

#include "cmyfunction.h"


/*!
* \brief  功能概述 构造函数
* \param  参数描述 无
* \return 返回值描述 无
* \author zzy
* \date   2015/1/16
*/
CMyFunction::CMyFunction()
{

}
int CMyFunction::ChannelNumber = -1;///< 当前窗口号 当前通道号
QList<DisplayFrame> CMyFunction::DisplayFrameList;///< 定义存储报文的QList
bool CMyFunction::IsShowTextBrowserWindow = false;///< 初始化报文窗口是否显示的标志位
int CMyFunction::TimeOut = 30;
QString CMyFunction::strProjectName = "";
QString CMyFunction::MonitorIPAddress="";
CProtocolI *CMyFunction::pMonitorProtocol = NULL;
QFile CMyFunction::m_sFile("RUN.log");
CRTDBSharedMemoryThread *CMyFunction::m_pRTDBSharedMemoryThread = NULL;
