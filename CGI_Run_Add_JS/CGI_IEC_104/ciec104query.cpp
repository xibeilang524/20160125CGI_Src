/*!
 * \file    ciec104query.cpp
 * \brief   概述 CIEC104Query类的源文件
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/5/25
 *
 * \b       修改记录：
 * \li      2015/5/25
 *  添加注释
 */

#include "ciec104query.h"

/*!
 * \brief  功能概述 构造函数
 * \param  参数描述 无
 * \return 返回值描述 无
 * \author zzy
 * \date   2015/5/25
 */
CIEC104Query::CIEC104Query()
{
}

/*!
 * \brief  功能概述 合成U帧，命令帧
 * \param  参数描述 bStart是开始标志，bStop是停止标志，bTest是测试标志
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CIEC104Query::MakeQuery_U_Command(bool bStart_, bool bStop_, bool bTest_)
{
    Q_ASSERT(bStart_+bStop_+bTest_ == 1);

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;

    *pCur++ = 0x68;
    *pCur++ = 4;
    *pCur = 3;
    if(bStart_)
        *pCur |= 4;
    if(bStop_)
        *pCur |= 0x10;
    if(bTest_)
        *pCur |= 0x40;
    pCur++;

    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = 0;

    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return Src_SetData(szSend,pCur-szSend);
}

/*!
 * \brief  功能概述 生成总召唤报文
 * \param  参数描述 nSendNo是发送序号，nReceiveNo是接收序号，nCommonAddress是公共地址
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CIEC104Query::MakeQuery_IntrogenCall(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress)
{
    //C_IC_NA_1 ACT
    //说明：格式为：
    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    *pCur++ = C_IC_NA_1;//0x64;//100 类型标识
    *pCur++ = 1;   //结构限定词
    *pCur++ = 6;   //传输原因 6=激活
    *pCur++ = 0; //传输原因高位

    *pCur++ = nCommonAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0; //公共地址高位

    *pCur++ = 0; //信息体地址低位
    *pCur++ = 0; //信息体地址高位
    *pCur++ = 0; //信息体地址高位

    *pCur++ = 0x14;//总召唤限定词(Qoi)

    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
}

/*!
 * \brief  功能概述 生成S帧
 * \param  参数描述 nReceiveNo是接收序号
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CIEC104Query::MakeQuery_S(WORD nReceiveNo)
{
    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    // TODO: Add extra operator here
    *pCur++ = 0x68;
    *pCur++ = 4;
    *pCur++ = 1;
    *pCur++ = 0;
    WORD nTemp = (nReceiveNo << 1);
    *pCur++ = LOBYTE(nTemp);
    *pCur++ = HIBYTE(nTemp);


    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return Src_SetData(szSend,pCur-szSend);
}

/*!
 * \brief  功能概述 生成累积量召唤报文
 * \param  参数描述 nSendNo是发送序号，nReceiveNo是接收序号，nCommonAddress是公共地址
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CIEC104Query::MakeQuery_CallPlus(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress)
{
    //C_CI_NA_1 ACT
    //说明：格式为：
    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    *pCur++ = C_CI_NA_1;//0x65;//101 类型标识
    *pCur++ = 1;   //结构限定词
    *pCur++ = 6;   //传输原因 6=激活
    *pCur++ = 0; //传输原因高位

    *pCur++ = nCommonAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0; //公共地址高位

    *pCur++ = 0; //信息体地址低位
    *pCur++ = 0; //信息体地址高位
    *pCur++ = 0; //信息体地址高位

    *pCur++ = 0x45;//请求电能总计数量QCC

    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
}

/*!
 * \brief  功能概述 生成校时命令
 * \param  参数描述 nSendNo是发送序号，nReceiveNo是接收序号，nCommonAddress是公共地址
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CIEC104Query::MakeQuery_AdjustTime(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress)
{
    //功能：对时报文
    //参数：nAddress 子站地址 nFcb  fcb状态
    //返回值：true
    //建立日期: 2007-08-29
    //C_CS_NA_1 ACT
    //说明：格式为：

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    *pCur++ = C_CS_NA_1;//0x67;//103 类型标识
    *pCur++ = 1;   //结构限定词
    *pCur++ = 6;   //传输原因 6=激活
    *pCur++ = 0; //传输原因高位

    *pCur++ = nCommonAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0; //公共地址高位

    *pCur++ = 0; //信息体地址低位
    *pCur++ = 0; //信息体地址高位
    *pCur++ = 0; //信息体地址高位

    ASDU101_CP56Time2a synTime;
    synTime.SetCurretTime();
    memcpy(pCur, &synTime, synTime.GetSize());
    pCur += synTime.GetSize();
    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);

}

/*!
 * \brief  功能概述 合成遥控报文 C_DC_NA_1 ACT
 * \param  参数描述 nSendNo是发送序号，nReceiveNo是接收序号，nCommonAddress是公共地址，nPoint是点号，bValue是分合状态，nStep (0　选择，1执行　2撤消)
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CIEC104Query::MakeQuery_RemoteControl(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress, int nPoint, bool bValue, int nStep)
{
    //建立日期: 2007-08-29
    //说明：格式为：
    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    *pCur++ = C_DC_NA_1;//0x2e; 46 类型标识 双位遥控命令
    *pCur++ = 1;   //结构限定词
    *pCur++ = nStep!=2?6:8;   //传输原因 6=激活  8停止激活
    *pCur++ = 0; //传输原因高位

    *pCur++ = nCommonAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0; //公共地址高位

    *pCur++ = LOBYTE(nPoint); //开关号低位
    *pCur++ = HIBYTE(nPoint); //开关号高位
    *pCur++ = 0; //信息体地址高位

    ASDU101_DCO *pDco = (ASDU101_DCO *)pCur++;
    pDco->QU = 0;
    if (bValue)
    {
        pDco->DCS = 2;// 合
    }
    else
    {
        pDco->DCS = 1;
    }
    if (nStep == 0)//选择
    {
        pDco->S_E = 1;
    }
    else
    {
        pDco->S_E = 0;
    }

    return MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
}

/*!
 \brief 合成遥设报文

 \fn CIEC104Query::MakeQuery_I_YS
 \param nSendNo
 \param nReceiveNo
 \param nCommonAddress
 \param nPoint
 \param nValue
 \return bool
*/
bool CIEC104Query::MakeQuery_I_YS(WORD &nSendNo, WORD nReceiveNo, BYTE nCommonAddress,ASDU101_TYPE nType, unsigned int nPointNumber, QVariant varValue)
{
    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    *pCur++ = nType;//
    *pCur++ = 1;   //结构限定词
    *pCur++ = 6;   //传输原因 6=激活  8停止激活
    *pCur++ = 0; //传输原因高位

    *pCur++ = nCommonAddress;//公共地址 (一般为子站地址)
    *pCur++ = 0; //公共地址高位

    *pCur++ = nPointNumber & 0x000000ff;        //信息体地址低位
    *pCur++ = (nPointNumber & 0x0000ff00)>>8;   //信息体地址高位
    *pCur++ = (nPointNumber & 0x00ff0000)>>16;  //信息体地址高位

    switch (nType) {
    case C_SE_NA_1:
    {
        ASDU101_NVA *pNva = (ASDU101_NVA *)pCur;
        pNva->m_nValue = varValue.toInt();
        pCur += pNva->GetSize();
    }
        break;
    case C_SE_NB_1:
    {
        ASDU101_SVA *pSva = (ASDU101_SVA *)pCur;
        pSva->m_nValue = varValue.toInt();
        pCur += pSva->GetSize();
    }
        break;
    case C_SE_NC_1:
    {
        ASDU101_R32 *pR32 = (ASDU101_R32 *)pCur;
        pR32->m_nValue = varValue.toFloat();
        pCur += pR32->GetSize();
    }
        break;
    case C_SE_ND_1:
    {

    }
        break;
    default:
        break;
    }

    ASDU101_QOS *pQos = (ASDU101_QOS *)pCur;
    pQos->QL = 0;
    pQos->SE = 0;///< 0 转发遥设直接执行
    pCur += pQos->GetSize();

    return MakeQuery_I(nSendNo,nReceiveNo,szSend, pCur-szSend);
}

/*!
 * \brief  功能概述 获取报文帧
 * \param  参数描述 无
 * \return 返回值描述 返回报文帧
 * \author zzy
 * \date   2015/5/25
 */
const QByteArray &CIEC104Query::GetByteArrayFrame()
{
    return m_ByteArrayFrame;
}

/*!
 * \brief  功能概述 合成I帧报文
 * \param  参数描述 nSendNo是发送序号，nReceiveNo是接收序号，pAsdu是asdu指针，nAsduSize是大小
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CIEC104Query::MakeQuery_I(WORD &nSendNo, WORD nReceiveNo, BYTE *pAsdu, int nAsduSize)
{
    Q_ASSERT(pAsdu != NULL);
    Q_ASSERT(nAsduSize>=0);
    Q_ASSERT(nAsduSize<=249);

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;
    *pCur++ = 0x68;
    *pCur++ = nAsduSize+4;

    WORD nTemp = (nSendNo <<1);
    nSendNo++;
    *pCur++ = LOBYTE(nTemp);
    *pCur++ = HIBYTE(nTemp);

    nTemp = (nReceiveNo <<1);
    *pCur++ = LOBYTE(nTemp);
    *pCur++ = HIBYTE(nTemp);

    memcpy(pCur,pAsdu,nAsduSize);
    pCur += nAsduSize;


    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return Src_SetData(szSend, pCur-szSend);
}

/*!
 * \brief  功能概述 合成U确认帧
 * \param  参数描述 bStart是开始标志，bStop是停止标志， bTest是测试标志
 * \return 返回值描述 成功返回true，失败返回false
 * \author zzy
 * \date   2015/5/25
 */
bool CIEC104Query::MakeQuery_U_Confirm(bool bStart_, bool bStop_, bool bTest_)
{
    Q_ASSERT(bStart_+bStop_+bTest_ == 1);

    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;

    *pCur++ = 0x68;
    *pCur++ = 4;
    *pCur = 3;
    if(bStart_)
        *pCur |= 8;
    if(bStop_)
        *pCur |= 0x20;
    if(bTest_)
        *pCur |= 0x80;
    pCur++;

    *pCur++ = 0;
    *pCur++ = 0;
    *pCur++ = 0;

    Q_ASSERT(BUFFER_SIZE >= pCur-szSend);
    return Src_SetData(szSend,pCur-szSend);
}

/*!
 * \brief  功能概述 将BYTE类型的数组转换成QByteArray类型的对象，存储在类中
 * \param  参数描述 pSource_是报文的头指针，nLength_是报文的总长度
 * \return 返回值描述 返回true
 * \author zzy
 * \date   2015/5/25
 */
bool CIEC104Query::Src_SetData(const BYTE *pSource_, int nLength_)
{
    m_ByteArrayFrame = QByteArray((const char*)pSource_,nLength_);
    return true;
}
