#ifndef CPOSTSET_H
#define CPOSTSET_H
#include <QByteArray>

#define MAX_BUFFER_CPOSTSET 10000
class CPostset
{
public:
    CPostset();
    static void HtmlToBytearray(char *src_,int nLen_, QByteArray &target_);
};

#endif // CPOSTSET_H
