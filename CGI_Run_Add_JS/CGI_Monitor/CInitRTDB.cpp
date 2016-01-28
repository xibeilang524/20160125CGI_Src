#include "CInitRTDB.h"
#include <QDomDocument>
#include <QDomElement>
#include "../PortInclude/cvaluebase.h"
#include "../PortInclude/CPointI.h"
#include "../PortInclude/CValueI.h"
#include "Src/ctagbase.h"
#include "Src/cmyfunction.h"
#include <QDateTime>

CRTDBBase g_RealTimeDB;

CInitRTDB::CInitRTDB(QStringList *pComFileList_, QStringList *pNetFileList_, QStringList *pCANFileList_, QObject *parent) :
    QObject(parent),
    m_pComFileList(pComFileList_),
    m_pNetFileList(pNetFileList_),
    m_pCANFileList(pCANFileList_),
    m_nTagID(0)
{

}

void CInitRTDB::InitRealTimeDB()
{
    ReadFilePoint(m_pComFileList);
    ReadFilePoint(m_pNetFileList);
    ReadFilePoint(m_pCANFileList);
}

void CInitRTDB::ReadFilePoint(QStringList *pStrFileNameList)
{
    if (!pStrFileNameList->isEmpty())
    {
        for (int i = 0; i < pStrFileNameList->count(); ++i)
        {
            ReadFilePoint(pStrFileNameList->at(i));
        }
    }
}

bool CInitRTDB::ReadFilePoint(QString strFileName)
{
    QDomDocument doc;
    if (CXmlParser::readFile(strFileName,doc))
    {
        qDebug()<<"\n**********************************************************\n"<<strFileName;
        QDomNodeList TypeElemList = doc.elementsByTagName("Type");
        CTagBase *pTagBase;// = new CTagBase(TagName,TagDesc,nID,);
        CValueI *pValue;
        for (int i = 0; i < TypeElemList.count(); ++i)
        {
            QDomElement TypeElem = TypeElemList.at(i).toElement();
            qDebug()<<TypeElem.tagName()<<TypeElem.attribute("Name");
            QDomNodeList PointList = TypeElem.elementsByTagName("Point");
            qDebug()<<"*********"<<TypeElem.parentNode().toElement().attribute("Name")<<"***********";
            QString TypeElem_Name_Attribute = TypeElem.attribute("Name");
            for (int nPointCount = 0; nPointCount < PointList.count(); ++nPointCount)
            {
                QDomElement PointElem = PointList.at(nPointCount).toElement();
                QString strLinkName;
                if (TypeElem_Name_Attribute == "YX")
                {/*Device_YX_Link_Strings*/

                    qDebug()<<TypeElem.attribute("Name")<<PointElem.attribute("Device_YX_Link_Strings")<<PointElem.attribute("Device_YX_Name_Strings");
                    strLinkName = PointElem.attribute("Device_YX_Link_Strings");
                    QVariant variant(0);///<zzy 2015/1/12 修改
                    pValue = new CValueBase;
                    pValue->SetVarValue(variant);
                    pTagBase = new CTagBase(TAG_TYPE_DI,strLinkName
                                            ,PointElem.attribute("Device_YX_Desc_Strings")
                                            ,m_nTagID++
                                            ,DATA_TYPE_INT32U
                                            ,pValue);

                }else if (TypeElem_Name_Attribute == "YC")
                {
                    qDebug()<<TypeElem.attribute("Name")<<PointElem.attribute("Device_YC_Link_Strings")<<PointElem.attribute("Device_YC_Name_Strings");
                    strLinkName = PointElem.attribute("Device_YC_Link_Strings");
                    QVariant variant(0);///<zzy 2015/1/12 修改
                    pValue = new CValueBase;
                    pValue->SetVarValue(variant);
                    pTagBase = new CTagBase(TAG_TYPE_AI,strLinkName
                                            ,PointElem.attribute("Device_YC_Desc_Strings")
                                            ,m_nTagID++
                                            ,DATA_TYPE_INT32U
                                            ,pValue);
                }else if (TypeElem_Name_Attribute == "YM")
                {
                    qDebug()<<TypeElem.attribute("Name")<<PointElem.attribute("Device_YM_Link_Strings")<<PointElem.attribute("Device_YM_Name_Strings");
                    strLinkName = PointElem.attribute("Device_YM_Link_Strings");
                    QVariant variant(0);///<zzy 2015/1/12 修改
                    pValue = new CValueBase;
                    pValue->SetVarValue(variant);
                    pTagBase = new CTagBase(TAG_TYPE_MEM,strLinkName
                                            ,PointElem.attribute("Device_YM_Desc_Strings")
                                            ,m_nTagID++
                                            ,DATA_TYPE_INT32U
                                            ,pValue);
                }else if (TypeElem_Name_Attribute == "YK")
                {
                    qDebug()<<TypeElem.attribute("Name")<<PointElem.attribute("Device_YK_Link_Strings")<<PointElem.attribute("Device_YK_Name_Strings");
                    strLinkName = PointElem.attribute("Device_YK_Link_Strings");
                    QVariant variant(0);///<zzy 2015/1/12 修改
                    pValue = new CValueBase;
                    pValue->SetVarValue(variant);
                    pTagBase = new CTagBase(TAG_TYPE_DO,strLinkName
                                            ,PointElem.attribute("Device_YK_Desc_Strings")
                                            ,m_nTagID++
                                            ,DATA_TYPE_INT32U
                                            ,pValue);
                }else if (TypeElem_Name_Attribute == "YS")
                {
                    qDebug()<<TypeElem.attribute("Name")<<PointElem.attribute("Device_YS_Link_Strings")<<PointElem.attribute("Device_YS_Name_Strings");
                    strLinkName = PointElem.attribute("Device_YS_Link_Strings");
                    QVariant variant(0);///<zzy 2015/1/12 修改
                    pValue = new CValueBase;
                    pValue->SetVarValue(variant);
                    pTagBase = new CTagBase(TAG_TYPE_AO,strLinkName
                                            ,PointElem.attribute("Device_YS_Desc_Strings")
                                            ,m_nTagID++
                                            ,DATA_TYPE_INT32U
                                            ,pValue);
                }
                if (!g_RealTimeDB.AddTag(strLinkName,pTagBase))
                {
                    m_nTagID--;
                }
            }
        }
    }
}
