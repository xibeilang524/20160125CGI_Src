#ifndef CDOWNLOADMAINWINDOW_H
#define CDOWNLOADMAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLineEdit>
#include <QProgressDialog>
#include <QDialogButtonBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>


namespace Ui {
class CDownLoadMainWindow;
}

class CDownLoadMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CDownLoadMainWindow(QWidget *parent = 0);
    ~CDownLoadMainWindow();
    void startRequest(QUrl url);
private slots:
    void downloadFile();
    void cancelDownload();
    void httpFinished();
    void httpReadyRead();
    void updateDataReadProgress(qint64 bytesRead, qint64 totalBytes);
    void enableDownloadButton();
    void slotAuthenticationRequired(QNetworkReply*,QAuthenticator *);
private:
    QLabel *statusLabel;
    QLabel *urlLabel;
    QLineEdit *urlLineEdit;
    QProgressDialog *progressDialog;
    QPushButton *downloadButton;
    QPushButton *quitButton;
    QDialogButtonBox *buttonBox;

    QUrl url;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    QFile *file;
    int httpGetId;
    bool httpRequestAborted;
private:
    Ui::CDownLoadMainWindow *ui;
};

#endif // CDOWNLOADMAINWINDOW_H
