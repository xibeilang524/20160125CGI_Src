#ifndef CGI_SCADA_DLL_COMMANDTHREAD_H
#define CGI_SCADA_DLL_COMMANDTHREAD_H

#include <QThread>

class CGI_SCADA_DLL_CommandThread : public QThread
{
    Q_OBJECT
public:
    explicit CGI_SCADA_DLL_CommandThread(QObject *parent = 0);

signals:

public slots:

};

#endif // CGI_SCADA_DLL_COMMANDTHREAD_H
