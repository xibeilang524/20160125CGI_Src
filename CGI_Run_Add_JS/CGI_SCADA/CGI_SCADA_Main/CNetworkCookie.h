#ifndef CNETWORKCOOKIE_H
#define CNETWORKCOOKIE_H

#include <QNetworkCookie>
#include <QNetworkCookieJar>

class CNetworkCookie : public QNetworkCookieJar
{
    Q_OBJECT
public:
    CNetworkCookie(QObject *parent = 0);
    ~CNetworkCookie();

    QList<QNetworkCookie> GetCookies();
    void SetCookies(const QList<QNetworkCookie>& cookieList);

signals:

public slots:

};

#endif // CNETWORKCOOKIE_H
