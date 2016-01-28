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
 \brief 构造函数

 \fn CXmlParser::CXmlParser
 \param parent 父类的指针
 \author zzy
 \date   2015/5/29
*/
CXmlParser::CXmlParser(QObject *parent) :
    QObject(parent)
{

}

/*!
 \brief 打开文件,并初始化QDomDocument

 \fn CXmlParser::readFile
 \param strFileName 用于打开的文件名
 \param doc 用于读入的QDomDocument
 \return bool 成功打开则返回true,打开失败则返回false
 \author zzy
 \date   2015/5/29
*/
bool CXmlParser::readFile(const QString strFileName, QDomDocument &doc)
{
    QFile file(strFileName);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QString strDebug = QString("不存在("+strFileName+")文件");
        qDebug()<<"不存在("<<strFileName<<")文件";
        return false;
    }
    QString errorStr;
    int errorLine;
    int errorColumn;
    if (!doc.setContent(&file, false, &errorStr, &errorLine, &errorColumn))
    {
        QString strDebug = QString("文件: "+strFileName+" 错误信息:%1 错误行号: %2 错误列号: %3").arg(errorStr).arg(errorLine).arg(errorColumn);
        qDebug()<<"文件: "<<strFileName<<" 错误信息:"<<errorStr<<" 错误行号:"<<errorLine<<" 错误列号:"<<errorColumn;
        return false;
    }
    file.close();
    return true;
}

/*!
 \brief 将doc中的数据写入文件中

 \fn CXmlParser::writeFile
 \param strFileName 用于写入的文件名
 \param doc 写入文件的数据源
 \return bool 成功写入则返回true,写入失败则返回false
 \author zzy
 \date   2015/5/29
*/
bool CXmlParser::writeFile(const QString strFileName, QDomDocument &doc)
{
    QFile file(strFileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QString strDebug = QString("Error: Cannot write file ("+strFileName+")文件"+"错误信息:"+file.errorString());
        qDebug()<<strDebug;
        return false;
    }

    const int indent = 4;///缩进
    QTextStream out(&file);
    out.reset();
    out.setCodec("utf-8");
    doc.save(out, indent, QDomNode::EncodingFromTextStream);
    file.close();

    return true;
}

/*!
 \brief 将doc中的数据写入文件中

 \fn CXmlParser::writeFile
 \param strFileName 用于写入的文件名
 \param doc 写入文件的数据源
 \return bool 成功写入则返回true,写入失败则返回false
*/
bool CXmlParser::writeFile(const QString &strFileName, const QDomDocument &doc)
{
    QFile file(strFileName);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QString strDebug = QString("Error: Cannot write file ("+strFileName+")文件"+"错误信息:"+file.errorString());
        qDebug()<<strDebug;
        return false;
    }

    const int indent = 4;///缩进
    QTextStream out(&file);
    out.reset();
    out.setCodec("utf-8");
    doc.save(out, indent, QDomNode::EncodingFromTextStream);
    file.close();

    return true;
}
