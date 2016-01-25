#ifndef UI_COM_ATTRIBUTE_H
#define UI_COM_ATTRIBUTE_H

#include <QWidget>
#include "qtpropertybrowser.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"
#include <QGridLayout>
#include "CGI_SCADA_DLL_define.h"
#include "../CGI_SCADA_include/CGI_SCADA_DLL_I.h"

namespace Ui {
class UI_COM_Attribute;
}

class UI_COMCAN_Attribute : public QWidget
{
    Q_OBJECT

public:
    explicit UI_COMCAN_Attribute(int nType_,QWidget *parent = 0);
    ~UI_COMCAN_Attribute();
    bool SaveAction(QDomDocument &doc, QDomElement &parentElem);
    bool SetComCanChannelTopProtocolMainPortElem(const QDomElement &ChannelElem_, const QDomElement &TopElem_,
                                           const QDomElement &ProtocolElem_,const QDomElement &MainPortElem_);

    bool SetNetLinkElem(const QDomElement &LinkElem_);
signals:
    void signal_ProtocolTypeChange(int);

private:
    void SetFileName_New();
    void SetConfigFileName(QString strFileName_ = "");
    void SetProtocolName(QString strProtocolName_);

private slots:
    void slot_propertyChange(QtProperty *property);
private:
    Ui::UI_COM_Attribute *ui;
private:
    QString m_strTabName;
    typedef QMap<int,AttributeParameter *> AttributeParameterMap;
    AttributeParameterMap *m_pParameterMap;///< 通道信息map
    AttributeParameterMap *m_pComParMap;///< 通讯参数map
    AttributeParameterMap *m_pProtocolParMap;///< 协议参数map
    int m_nTreeItemType;///< 树节点类型
    QString m_strName;///< 节点名称
    QString m_strProtocolName;///< 驱动名称

    QGridLayout *m_pLayout;
    QtProperty *topItem;///< 上部的通道信息指针
    QtProperty *m_pLeftItem;///< 左侧的通讯方式指针
    QtProperty *m_pRightItem;///< 右侧的协议参数指针

    QtTreePropertyBrowser *m_pTopVariantEditor;///< 树形属性浏览器
    QtTreePropertyBrowser *m_pLeftVariantEditor;///< 树形属性浏览器
    QtTreePropertyBrowser *m_pRightVariantEditor;///< 树形属性浏览器

    QtVariantPropertyManager *m_pTopVariantManager;
    QtVariantPropertyManager *m_pLeftVariantManager;
    QtVariantPropertyManager *m_pRightVariantManager;

    int m_nType;
};

#endif // UI_COM_ATTRIBUTE_H
