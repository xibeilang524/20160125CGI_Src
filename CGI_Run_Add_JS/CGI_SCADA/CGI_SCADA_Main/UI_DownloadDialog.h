#ifndef UI_DOWNLOADDIALOG_H
#define UI_DOWNLOADDIALOG_H

#include <QDialog>
#include <QUrl>
class QFile;
class QNetworkReply;
class QNetworkAccessManager;

namespace Ui {
class UI_DownloadDialog;
}

class UI_DownloadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UI_DownloadDialog(QWidget *parent = 0);
    ~UI_DownloadDialog();

private:
    Ui::UI_DownloadDialog *ui;
public:
    void startRequest(QUrl url);

private:
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QUrl url;
    QFile *file;
private slots:
    void replyFinished(QNetworkReply *);
    void httpFinished();
    void httpReadyRead();
    void updateDataReadProgress(qint64,qint64);
    void on_pushButton_clicked();
};

#endif // UI_DOWNLOADDIALOG_H
