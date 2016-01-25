#ifndef CMAINWINDOW_H
#define CMAINWINDOW_H

#include <QMainWindow>
#include <QWebFrame>
#include <QWebView>

namespace Ui {
class CMainWindow;
}

class CMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CMainWindow(QWidget *parent = 0);
    ~CMainWindow();

private slots:
    void slot_urlChanged(QUrl url);
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::CMainWindow *ui;
    QWebView *m_pWebView;
    QList<QUrl> m_UrlList;
    QUrl m_CurrentUrl;
    QUrl m_PreviousUrl;
    int m_nCurrentUrl;
    int m_nBackspaceTimes;
    bool m_IsBackStatus;
    bool m_IsBackStatus_2;
};

#endif // CMAINWINDOW_H
