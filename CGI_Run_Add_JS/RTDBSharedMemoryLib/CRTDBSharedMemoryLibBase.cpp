#include "CRTDBSharedMemoryLibBase.h"
#include "../PortInclude/XmlParser/CXmlParser.h"
#include <QDomDocument>
#include <QDebug>
#include <QDateTime>
#include <QDir>
/*!
 * \brief 功能概述 库文件导出接口函数
 * \param 参数描述 parent是QObject对象指针
 * \return 返回值描述 返回创建的驱动指针
 * \author zzy
 * \date 2015/1/14
 */
CRTDBSharedMemoryLibI * CreateDriver(QObject *parent)
{
    ///库文件导出函数
    CRTDBSharedMemoryLibBase *testProtocol = new CRTDBSharedMemoryLibBase(parent);
    return  testProtocol;
}
CRTDBSharedMemoryLibBase::CRTDBSharedMemoryLibBase(QObject */*parent*/)
{
    m_isSucceed = false;
    connect(this,SIGNAL(Signal_SetValue(int,QVariant)),this,SLOT(Slot_SetValue(int,QVariant)));
}

bool CRTDBSharedMemoryLibBase::Create(QMap<int, CSharedMemoryTag *> *pRTDB_, QString /*strFileName_*/)
{
    m_pRTDB = pRTDB_;
    m_SharedMemory.setKey("SCADA_RTDB_SharedMemoryLib");
    QDomDocument SCADA_RTDB_SharedMemoryLib;
    QDomElement Root = SCADA_RTDB_SharedMemoryLib.createElement(tr("Root"));
    SCADA_RTDB_SharedMemoryLib.appendChild(Root);
    m_nMaxTagIndex = 0;


    QMap<int, CSharedMemoryTag *>::iterator iterator;
    for (iterator = pRTDB_->begin(); iterator != pRTDB_->end(); ++iterator)
    {
        int nPointNumber = iterator.value()->m_nTagIndex;
//        qDebug()<<"nPointNumber"<<nPointNumber;
        if (m_nMaxTagIndex < nPointNumber)
        {
            m_nMaxTagIndex = nPointNumber;
        }
        QDomElement Tag = SCADA_RTDB_SharedMemoryLib.createElement(tr("Tag"));
        Tag.setAttribute("TagName",iterator.value()->m_strTagRTDBName);
        Tag.setAttribute("TagIndex",iterator.value()->m_nTagIndex);
        Tag.setAttribute("TagType",iterator.value()->m_nTagType);
        qDebug()<<__FILE__<<__func__<<__LINE__<<"TagName"<<iterator.value()->m_strTagRTDBName
               <<"TagIndex"<<iterator.value()->m_nTagIndex<<"TagType"<<iterator.value()->m_nTagType;
        Root.appendChild(Tag);
    }
    CXmlParser::writeFile(tr("SCADA_RTDB_SharedMemoryLib.xml"),SCADA_RTDB_SharedMemoryLib);
    m_nSize = (m_nMaxTagIndex+10)*(4+4+24)+1024;///< 每个变量占用32字节 前4个字节是值类型，中间4个字节是实时值，后24个字节是时标
    if (m_nSize < 1024*30)
    {
        m_nSize = 1024*30;
    }
    if (m_SharedMemory.create(m_nSize,QSharedMemory::ReadWrite))
    {
        m_isSucceed = true;
        qDebug()<<"SCADA_RTDB_SharedMemoryLib: Create succeed"<<m_nSize<<m_nMaxTagIndex;
        /// 设置开始8个字节的数据
//        int nPianYi = 0;
//        int nValue = m_nMaxTagIndex;
//        char *to = (char*)m_SharedMemory.data();

        // Write into the shared memory
//        m_SharedMemory.lock();
//        *(int *)(to+((nPianYi)*sizeof(int))) = nValue;
//        qDebug()<<"xie ru le ";
//        m_SharedMemory.unlock();
        qDebug()<<"Create sharedMemory secceed."<<m_SharedMemory.key()<<m_SharedMemory.nativeKey();
        return true;
    }else
    {
        qDebug()<<"Create sharedMemory failed."<<m_SharedMemory.key()<<m_SharedMemory.nativeKey()<<m_SharedMemory.errorString()<<m_SharedMemory.error()<<m_nSize;
        m_isSucceed = false;
        return false;
    }
}

bool CRTDBSharedMemoryLibBase::WritePointFile()
{
    return true;
}
bool CRTDBSharedMemoryLibBase::ReadPointFile()
{
    return true;
}

bool CRTDBSharedMemoryLibBase::Open()
{
    m_pRTDB = new QMap<int,CSharedMemoryTag *>;
#if defined(PRINTF)
        printf("BBBBBBBBBBBBBBBBBBBBBBBBAA %s\n",QDir::currentPath().toStdString().data());
#else

#endif
    qDebug()<<"BBBBBBBBBBBBBBBBBBBBBBBBAA"<<QDir::currentPath();
    QDomDocument SCADA_RTDB_SharedMemoryLib;
//    CXmlParser::readFile("SCADA_RTDB_SharedMemoryLib.xml",SCADA_RTDB_SharedMemoryLib);
    if (CXmlParser::readFile("SCADA_RTDB_SharedMemoryLib.xml",SCADA_RTDB_SharedMemoryLib))
    {
#if defined(PRINTF)
        printf("dakai1\n");
#else

#endif
        qDebug()<<"dakai1";
    }else
    {
#if defined(PRINTF)
        printf("weidakai1\n");
#else

#endif
        qDebug()<<"weidakai1";
        if (CXmlParser::readFile("cgi-bin/SCADA_RTDB_SharedMemoryLib.xml",SCADA_RTDB_SharedMemoryLib))
        {
#if defined(PRINTF)
        printf("dakai2\n");
#else

#endif
            qDebug()<<"dakai2";
        }else
        {
#if defined(PRINTF)
        printf("weidakai2\n");
#else

#endif
            qDebug()<<"weidakai2";
        }
    }



    QDomNodeList SharedMemoryTag = SCADA_RTDB_SharedMemoryLib.elementsByTagName("Tag");
    for (int i = 0; i < SharedMemoryTag.count(); ++i)
    {
        QDomElement TagEle = SharedMemoryTag.at(i).toElement();
        int nTagIndex = TagEle.attribute("TagIndex").toInt();
        CSharedMemoryTag *pSharedMemoryTag = new CSharedMemoryTag(nTagIndex,
                                                                  TagEle.attribute("TagType").toInt(),
                                                                  TagEle.attribute("TagName"));
        if(!m_pRTDB->contains(nTagIndex))
        {
//            qDebug()<<TagEle.attribute("TagName")<<"chenggong";
            m_pRTDB->insert(nTagIndex, pSharedMemoryTag);
        }else
        {
//            qDebug()<<TagEle.attribute("TagName")<<"shibai";
        }
    }
//E:/web-test/cgi-bin"
//    m_SharedMemory.setKey("qipc_sharedmemory_SCADARTDBSharedMemoryLib67fbe49ab3c0607924770bd28db461ea40d63286");
//    m_SharedMemory.setNativeKey("qipc_sharedmemory_SCADARTDBSharedMemoryLib67fbe49ab3c0607924770bd28db461ea40d63286");
    m_SharedMemory.setKey("SCADA_RTDB_SharedMemoryLib");
    if (m_SharedMemory.attach())
    {
        m_isSucceed = true;
#if defined(PRINTF)
        printf("attach yi fu secceed. %s %s\n",m_SharedMemory.key().toStdString().data(),m_SharedMemory.nativeKey().toStdString().data());
#else

#endif
        qDebug()<<"attach yi fu secceed."<<m_SharedMemory.key()<<m_SharedMemory.nativeKey();
        return true;
    }else
    {
#if defined(PRINTF)
        printf("attach yi fu failed. %s %s %d %s\n",m_SharedMemory.key().toStdString().data(),m_SharedMemory.nativeKey().toStdString().data(),
               m_SharedMemory.error(),m_SharedMemory.errorString().toStdString().data());
#else

#endif
        qDebug()<<"attach yi fu failed"<<m_SharedMemory.key()<<m_SharedMemory.nativeKey()<<m_SharedMemory.error()<<m_SharedMemory.errorString();/// yi fu shi bai
#if 0
        m_SharedMemory.setKey("SCADA_RTDB_SharedMemoryLib");
        if (m_SharedMemory.create(5000,QSharedMemory::ReadWrite))
        {
            m_isSucceed = true;
            qDebug()<<"SCADA_RTDB_SharedMemoryLib: Create succeed"<<m_nSize<<m_nMaxTagIndex;
            /// 设置开始8个字节的数据
    //        int nPianYi = 0;
    //        int nValue = m_nMaxTagIndex;
    //        char *to = (char*)m_SharedMemory.data();

            // Write into the shared memory
    //        m_SharedMemory.lock();
    //        *(int *)(to+((nPianYi)*sizeof(int))) = nValue;
    //        qDebug()<<"xie ru le ";
    //        m_SharedMemory.unlock();
            qDebug()<<"22Create sharedMemory secceed."<<m_SharedMemory.key()<<m_SharedMemory.nativeKey();
            return true;
        }else
        {
            qDebug()<<"22Create sharedMemory faild."<<m_SharedMemory.key()<<m_SharedMemory.nativeKey();
            m_isSucceed = false;
            return false;
        }
#endif
        m_isSucceed = false;
        qDebug()<<"attach yi fu failed"<<m_SharedMemory.key()<<m_SharedMemory.nativeKey()<<m_SharedMemory.error()<<m_SharedMemory.errorString();/// yi fu shi bai
        return false;
    }
    return true;
}

bool CRTDBSharedMemoryLibBase::SetValue(int nID_, QVariant varValue_)
{
//    qDebug()<<__func__<<__FILE__<<3;
    emit Signal_SetValue(nID_, varValue_);
    return true;
}

QList<CSharedMemoryPoint> CRTDBSharedMemoryLibBase::GetValue(QList<int> nIDList_)
{
    QList<CSharedMemoryPoint> ValueListResult;
    if (m_isSucceed)
    {
        for (int i = 0; i < nIDList_.size(); ++i)
        {
            int nIndex = nIDList_.at(i);

            CSharedMemoryPoint sharedMemoryPoint;
            CSharedMemoryTag *pSharedMemoryTag = m_pRTDB->value(nIndex);
            if(pSharedMemoryTag)
            {
                int nPianYi = pSharedMemoryTag->m_nTagIndex * 32;
                char *to = (char*)m_SharedMemory.data();
                if (to == NULL)
                {
                    return ValueListResult;
                }
                QString strDateTime;
                sharedMemoryPoint.m_nPointIndex = nIndex;

                m_SharedMemory.lock();
                int nType = *(int *)(to+nPianYi);

                switch (nType) {
                case QVariant::Bool:
                {
                    int bValue = *(int *)(to + nPianYi + 4);
                    QVariant var(bValue);
                    sharedMemoryPoint.m_nVariant = var;
                }
                    break;
                case QVariant::UInt:
                {
                    uint uValue = *(uint *)(to + nPianYi + 4);
                    QVariant var(uValue);
                    sharedMemoryPoint.m_nVariant = var;
                }
                    break;
                case QVariant::Int:
                {
                    int nValue = *(int *)(to + nPianYi + 4);
                    QVariant var(nValue);
                    sharedMemoryPoint.m_nVariant = var;
                }
                    break;
                case QMetaType::Float:
                {
                    float fValue = *(float *)(to + nPianYi + 4);
                    QVariant var(fValue);
                    sharedMemoryPoint.m_nVariant = var;
                }
                    break;
                default:
                {
                    int nValue = *(int *)(to + nPianYi + 4);
                    QVariant var(nValue);
                    sharedMemoryPoint.m_nVariant = var;
                }
                    break;
                }
                strDateTime = QString( (char *)(to+nPianYi+8) );
//                qDebug()<<"du qu le"<<nIndex;
                m_SharedMemory.unlock();
                sharedMemoryPoint.m_strDateTime = strDateTime;
                sharedMemoryPoint.m_isValid = true;
            }else
            {
                sharedMemoryPoint.m_isValid = false;
                sharedMemoryPoint.m_nPointIndex = nIndex;
                sharedMemoryPoint.m_nVariant = QVariant();
                sharedMemoryPoint.m_strDateTime = QString();
            }

            ValueListResult.append(sharedMemoryPoint);
        }
    }
    return ValueListResult;
}

CSharedMemoryPoint CRTDBSharedMemoryLibBase::GetValue(int nID_)
{
    CSharedMemoryPoint sharedMemoryPoint;
    if (m_isSucceed)
    {
        CSharedMemoryTag *pSharedMemoryTag = m_pRTDB->value(nID_);
        if(pSharedMemoryTag)
        {
            int nPianYi = pSharedMemoryTag->m_nTagIndex * 32;
            char *to = (char*)m_SharedMemory.data();
            if (to == NULL)
            {
                return sharedMemoryPoint;
            }
            QString strDateTime;
            sharedMemoryPoint.m_nPointIndex = nID_;

            m_SharedMemory.lock();
            int nType = *(int *)(to+nPianYi);
            switch (nType) {
            case QVariant::Bool:
            {
                int bValue = *(int *)(to + nPianYi + 4);
                QVariant var(bValue);
                sharedMemoryPoint.m_nVariant = var;
            }
                break;
            case QVariant::UInt:
            {
                uint uValue = *(uint *)(to + nPianYi + 4);
                QVariant var(uValue);
                sharedMemoryPoint.m_nVariant = var;
            }
                break;
            case QVariant::Int:
            {
                int nValue = *(int *)(to + nPianYi + 4);
                QVariant var(nValue);
                sharedMemoryPoint.m_nVariant = var;
            }
                break;
            case QMetaType::Float:
            {
                float fValue = *(float *)(to + nPianYi + 4);
                QVariant var(fValue);
                sharedMemoryPoint.m_nVariant = var;
            }
                break;
            default:
            {
                int nValue = *(int *)(to + nPianYi + 4);
                QVariant var(nValue);
                sharedMemoryPoint.m_nVariant = var;
            }
                break;
            }
            strDateTime = QString( (char *)(to+nPianYi+8) );
//            qDebug()<<"du qu le"<<nID_<<strDateTime;
            m_SharedMemory.unlock();
            sharedMemoryPoint.m_strDateTime = strDateTime;
            sharedMemoryPoint.m_isValid = true;
            return sharedMemoryPoint;
        }else
        {
            return CSharedMemoryPoint(nID_);
        }
    }else
    {
        return CSharedMemoryPoint(nID_);
    }
}

CSharedMemoryPoint CRTDBSharedMemoryLibBase::GetValue(QString strRTDBName_)
{
//    strRTDBName_
    CSharedMemoryPoint sharedMemoryPoint;
    int nID_ = -1;
    if (m_isSucceed)
    {

        QMap<int, CSharedMemoryTag *>::iterator iterator;
        CSharedMemoryTag *pSharedMemoryTag = NULL;
        for (iterator = m_pRTDB->begin(); iterator != m_pRTDB->end(); ++iterator)
        {
            qDebug()<<iterator.value()->m_strTagRTDBName<<strRTDBName_;
            if (iterator.value()->m_strTagRTDBName == strRTDBName_)
            {
                pSharedMemoryTag = iterator.value();
                nID_ = iterator.value()->m_nTagIndex;
                break;
            }
        }
//        qDebug()<<"nID_ ==== "<<nID_;
        if(pSharedMemoryTag)
        {
            qDebug()<<" if(pSharedMemoryTag)"<<pSharedMemoryTag->m_nTagIndex<<pSharedMemoryTag->m_strTagRTDBName;
            int nPianYi = pSharedMemoryTag->m_nTagIndex * 32;
            char *to = (char*)m_SharedMemory.data();
            if (to == NULL)
            {
                return sharedMemoryPoint;
            }
            QString strDateTime;
            sharedMemoryPoint.m_nPointIndex = nID_;

            m_SharedMemory.lock();
            int nType = *(int *)(to+nPianYi);
            switch (nType) {
            case QVariant::Bool:
            {
                int bValue = *(int *)(to + nPianYi + 4);
                QVariant var(bValue);
                sharedMemoryPoint.m_nVariant = var;
            }
                break;
            case QVariant::UInt:
            {
                uint uValue = *(uint *)(to + nPianYi + 4);
                QVariant var(uValue);
                sharedMemoryPoint.m_nVariant = var;
            }
                break;
            case QVariant::Int:
            {
                int nValue = *(int *)(to + nPianYi + 4);
                QVariant var(nValue);
                sharedMemoryPoint.m_nVariant = var;
            }
                break;
            case QMetaType::Float:
            {
                float fValue = *(float *)(to + nPianYi + 4);
                QVariant var(fValue);
                sharedMemoryPoint.m_nVariant = var;
            }
                break;
            default:
            {
                int nValue = *(int *)(to + nPianYi + 4);
                QVariant var(nValue);
                sharedMemoryPoint.m_nVariant = var;
            }
                break;
            }
            strDateTime = QString( (char *)(to+nPianYi+8) );
//            qDebug()<<"du qu le"<<nID_<<strDateTime;
            m_SharedMemory.unlock();
            sharedMemoryPoint.m_strDateTime = strDateTime;
            sharedMemoryPoint.m_isValid = true;
            return sharedMemoryPoint;
        }else
        {
            return CSharedMemoryPoint(nID_);
        }
    }else
    {
        return CSharedMemoryPoint(nID_);
    }
}

bool CRTDBSharedMemoryLibBase::Slot_SetValue(int nID_, QVariant varValue_)
{
//    qDebug()<<__func__<<__FILE__<<4<<m_isSucceed;
    if (m_isSucceed)
    {
        CSharedMemoryTag *pSharedMemoryTag = m_pRTDB->value(nID_);
//        qDebug()<<"nID_11 ="<<nID_<<"varValue_ ="<<varValue_;
        if(pSharedMemoryTag != NULL)
        {
            int nPianYi = pSharedMemoryTag->m_nTagIndex * 32;
            char *to = (char*)m_SharedMemory.data();
            if (to == NULL)
            {
                return false;
            }
//            qDebug()<<"nID_22 ="<<nID_<<"varValue_ ="<<varValue_;

            switch (varValue_.type())
            {
                case QVariant::Bool:     //0   bool
                {
                    // Write into the shared memory
                    bool bValue = varValue_.toBool();
//                    qDebug()<<"bool"<<varValue_<<bValue;
                    m_SharedMemory.lock();
                    *(int *)(to+(nPianYi)) = QVariant::Bool;
                    *(int *)(to+(nPianYi+4)) = bValue;
//                    qDebug()<<"xie ru le 11111";
                    m_SharedMemory.unlock();
                }
                    break;
                case QVariant::UInt:      //5   4字节无符号数
                {
                    // Write into the shared memory
                    unsigned int unValue = varValue_.toUInt();
//                    qDebug()<<"unValue"<<varValue_<<unValue;
                    m_SharedMemory.lock();
                    *(int *)(to+(nPianYi)) = QVariant::UInt;
                    *(unsigned int *)(to+(nPianYi+4)) = unValue;
//                    qDebug()<<"xie ru le 11111";
                    m_SharedMemory.unlock();
                }
                    break;
                case QVariant::Int:      //6   4字节有符号数
                {
                    // Write into the shared memory
                    int nValue = varValue_.toInt();
//                    qDebug()<<"nValue"<<varValue_<<nValue;
                    m_SharedMemory.lock();
                    *(int *)(to+(nPianYi)) = QVariant::Int;
                    *(int *)(to+(nPianYi+4)) = nValue;
//                    qDebug()<<"xie ru le 11111";
                    m_SharedMemory.unlock();
                }
                    break;
                case QVariant::Double:        //11  4字节浮点数
                {
                    // Write into the shared memory
                    double fValue = varValue_.toDouble();
//                    qDebug()<<"fValue"<<varValue_<<fValue;
                    m_SharedMemory.lock();
                    *(int *)(to+(nPianYi)) = QMetaType::Float;
                    *(double *)(to+(nPianYi+4)) = fValue;
//                    qDebug()<<"xie ru le 11111";
                    m_SharedMemory.unlock();
                }
                    break;
                default:
                {
                    // Write into the shared memory
                    int nValue = varValue_.toInt();
//                    qDebug()<<"nValue"<<varValue_<<nValue;
                    m_SharedMemory.lock();
                    *(int *)(to+(nPianYi)) = QVariant::Int;
                    *(int *)(to+(nPianYi+4)) = nValue;
//                    qDebug()<<"xie ru le 11111";
                    m_SharedMemory.unlock();
                    break;
                }
            }
            /// 设置时标
            QString strDateTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz");
            memcpy(to+nPianYi+8,strDateTime.toUtf8().data(),strDateTime.toUtf8().count());
//            qDebug()<<"nID_ ="<<nID_<<"设置时标"<<(char *)(to+nPianYi+8)<<nID_;
            return true;
        }else
        {
            return false;
        }
    }else
    {
        return false;
    }
}


