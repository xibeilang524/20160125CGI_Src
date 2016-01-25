#include "CPostset.h"
#include <QDebug>

CPostset::CPostset()
{
}

void CPostset::HtmlToBytearray(char *src_, int nLen_, QByteArray &target_)
{
    char *p = src_;
    for (int i = 0; i < nLen_;++i)
    {
        if (*p == '%')
        {
            QByteArray aaa(p+1,2);
//            qDebug()<<"aaa"<<aaa.fromHex(aaa);
            i+=2;
            p+=2;
            target_.append(aaa.fromHex(aaa));
        }else
        {
            target_.append(p,1);
        }
        p++;
//        qDebug()<<i<<*p;
    }
}
