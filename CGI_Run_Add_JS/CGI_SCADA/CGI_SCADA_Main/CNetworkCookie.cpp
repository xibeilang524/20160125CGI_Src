#include "CNetworkCookie.h"

CNetworkCookie::CNetworkCookie(QObject *parent) :
    QNetworkCookieJar(parent)
{
}

CNetworkCookie::~CNetworkCookie()
{

}

QList<QNetworkCookie> CNetworkCookie::GetCookies()
{
    return allCookies();
}

void CNetworkCookie::SetCookies(const QList<QNetworkCookie> &cookieList)
{
    if(this == NULL)
        return;
    this->setAllCookies(cookieList);
}
