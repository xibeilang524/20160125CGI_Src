#ifndef CGI_SCADA_DEFINE_H
#define CGI_SCADA_DEFINE_H

#include <QObject>
#include <QString>
#include <QByteArray>

class CGI_SCADA_define
{
public:
    static QByteArray s_baCookie;
    static QString s_strIPAddress;
};

#endif // CGI_SCADA_DEFINE_H
