/*!
 * \file  CXmlParser.h
 * \brief 概述 CXmlParser类的头文件
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

#ifndef CXMLPARSER_H
#define CXMLPARSER_H

#include <QObject>
#include <QDomElement>
#include <QDomDocument>
#include <QDebug>

/*!
 \brief 与两个用于打开文件和写文件的静态函数

 \class CXmlParser CXmlParser.h "SCADA/XmlParser/CXmlParser.h"
 \author zzy
 \date   2015/5/29
*/
class CXmlParser : public QObject
{
    Q_OBJECT
public:
    explicit CXmlParser(QObject *parent = 0);
    static bool readFile (const QString strFileName,  QDomDocument &doc);
    static bool writeFile(const QString strFileName,  QDomDocument &doc);
    static bool writeFile(const QString &strFileName, const QDomDocument &doc);
signals:

public slots:

};

#endif // CXMLPARSER_H
