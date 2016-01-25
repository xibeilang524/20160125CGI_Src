#ifndef CTREEWIDGETITEM_H
#define CTREEWIDGETITEM_H

#include <QTreeWidgetItem>
#include "CChannelMessage.h"
#include "EnumClass.h"

class CTreeWidgetItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    CTreeWidgetItem(int nChannelNumber_,QString strText_,QDomElement element_,QTreeWidget * parent, int type = Type);
    CTreeWidgetItem(QDomElement element_,QTreeWidgetItem * parent, int type = Type);
    CTreeWidgetItem(int nChannelNumber_,QDomElement element_,QTreeWidgetItem * parent, int type = Type);
    int GetChannelNumber() const;
    QStandardItemModel *GetModel(ChannelMessage_TAG_TYPE nTagType_);
signals:

public slots:

private:
    CChannelMessage *m_pChannelMessage;
    int m_nChannelNumber;

};

#endif // CTREEWIDGETITEM_H
