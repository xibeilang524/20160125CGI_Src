#ifndef UI_LOGIN_DIALOG_H
#define UI_LOGIN_DIALOG_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "CNetworkCookie.h"
#include "../CGI_SCADA_include/CGI_SCADA_define.h"
namespace Ui {
class UI_Login_Dialog;
}
class UI_Login_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit UI_Login_Dialog(QWidget *parent = 0);
    ~UI_Login_Dialog();
    bool GetLoginStatus();
    QByteArray GetLoginCookie();
    QStringList GetTreeItemDllName();

private slots:
    void replyFinished(QNetworkReply*);
    void on_pushButton_clicked();

private:
    Ui::UI_Login_Dialog *ui;
    bool m_bLoginStatus;
    QNetworkAccessManager *m_pManager;
    QNetworkReply *m_pReply;
    CNetworkCookie network_cookie_;
    QByteArray m_baCookie;
};

#endif // UI_LOGIN_DIALOG_H
