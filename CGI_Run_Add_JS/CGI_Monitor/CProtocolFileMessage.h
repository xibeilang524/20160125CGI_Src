#ifndef CPROTOCOLFILEMESSAGE_H
#define CPROTOCOLFILEMESSAGE_H

#include <QObject>
#include "EnumClass.h"

class CProtocolFileMessage : public QObject
{
    Q_OBJECT
public:
    explicit CProtocolFileMessage(QObject *parent = 0);
    static void InitProtocolFileMessage(QString strFileName_);
    static QMap<QString, PropertyList*> m_ProtocolMessageMap;
signals:

public slots:

};

#endif // CPROTOCOLFILEMESSAGE_H
