#ifndef UI_DEVICEATTRIBUTE_H
#define UI_DEVICEATTRIBUTE_H

#include <QWidget>
#include "CGI_SCADA_DLL_define.h"
#include "qtpropertybrowser.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"

namespace Ui {
class UI_DeviceAttribute;
}

class UI_DeviceAttribute : public QWidget
{
    Q_OBJECT

public:
    explicit UI_DeviceAttribute(QList<Device_Att_Parameter> deviceAttParameterList_,QWidget *parent = 0);
    ~UI_DeviceAttribute();
    bool SaveAction(QDomDocument &doc, QDomElement &parentElem);

    bool SetDeviceAttributeElem(const QDomElement &AttributeElem_);
private:
    Ui::UI_DeviceAttribute *ui;
    QList<Device_Att_Parameter> m_DeviceAttParameterList;
    QGridLayout *m_pLayout;
    QtProperty *topItem;///< 上部的通道信息指针
    QtTreePropertyBrowser *m_pTopVariantEditor;///< 树形属性浏览器
    QtVariantPropertyManager *m_pTopVariantManager;

    void SetParameterList();
};

#endif // UI_DEVICEATTRIBUTE_H
