/*!
 * \file  CXmlParser.cpp
 * \brief 概述 CXmlParser类的源文件
 *
 *详细概述
 *
 * \author zzy
 * \version 版本号
 * \date 2015/1/9
 *
 * \b 修改记录：
 * \li 2015/1/9
 *  添加注释
 */

#include "CXmlParser.h"
#include <QDomDocument>
#include <QFile>
#include <QDebug>

/*!
* \brief 功能概述 构造函数
* \param 参数描述 parent父类的指针
* \return 返回值描述 无
* \author zzy
* \date 2015/1/9
*/
CXmlParser::CXmlParser(QObject *parent) :
    QObject(parent)
{

}

/*!
 * \brief 功能概述 打开文件,并初始化QDomDocument
 * \param 参数描述 strFileName用于打开的文件名,doc用于读入的QDomDocument
 * \return 返回值描述 成功打开则返回true,打开失败则返回false
 * \author zzy
 * \date 2015/1/9
 */
bool CXmlParser::readFile(const QString strFileName, QDomDocument &doc)
{

    QFile file(strFileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QString strDebug = QString("don't ("+strFileName+")file.");
        qDebug()<<strDebug;
//        Q_ASSERT_X(false,"bool CXmlParser::readFile(const QString strFileName, QDomDocument &doc)",strDebug.toUtf8());
        return false;
    }
    QString errorStr;
    int errorLine;
    int errorColumn;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        QString strDebug = QString("File: "+strFileName+" Error Message :%1 Error Line: %2 Error Column: %3").arg(errorStr).arg(errorLine).arg(errorColumn);
        qDebug()<<strDebug;
//        Q_ASSERT_X(false,"bool CXmlParser::readFile(const QString strFileName, QDomDocument &doc)",strDebug.toUtf8());
        return false;
    }
    file.close();
    return true;
}

/*!
 * \brief 功能概述 将doc中的数据写入文件中
 * \param 参数描述 strFileName用于写入的文件名,doc写入文件的数据源
 * \return 返回值描述 成功写入则返回true,写入失败则返回false
 * \author zzy
 * \date 2015/1/9
 */
bool CXmlParser::writeFile(const QString strFileName, QDomDocument &doc)
{
    QFile file(strFileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QString strDebug = QString("Error: Cannot write file ("+strFileName+")File"+"Error Message:"+file.errorString());
        qDebug()<<strDebug;
        Q_ASSERT_X(false,"bool CXmlParser::writeFile(const QString strFileName, QDomDocument &doc)",strDebug.toUtf8());
        return false;
    }

    const int indent = 4;///< 缩进
    QTextStream out(&file);
    out.reset();
    out.setCodec("utf-8");
    doc.save(out, indent, QDomNode::EncodingFromTextStream);
    file.close();

    return true;
}
