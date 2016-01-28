#ifndef CTREEITEMTHREAD_H
#define CTREEITEMTHREAD_H

#include <QThread>
#include <QLibrary>

class CGI_SCADA_DLL_I;
class CTreeItemThread : public QThread
{
    Q_OBJECT
public:
    explicit CTreeItemThread(QObject *parent = 0);
    void run() Q_DECL_OVERRIDE;
    void SetLibraryName(const QString &strLibName_);
    CGI_SCADA_DLL_I *GetTreeItem();
    bool SetStart();

signals:
    void signal_ShowMessageDebug(int nType_,QString strMessage);
    void signal_OpenProject(QString strProjectPath);

public slots:
private:
    CGI_SCADA_DLL_I *m_pProtocolI;///< 库中导出的类
    QString m_LibName;///< 库文件的路径
    QLibrary mylib;///< 库文件
};

#endif // CTREEITEMTHREAD_H
