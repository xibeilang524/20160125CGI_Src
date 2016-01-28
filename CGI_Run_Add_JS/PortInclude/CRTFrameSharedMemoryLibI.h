#ifndef CRTFRAMESHAREDMEMORYLIBI_H
#define CRTFRAMESHAREDMEMORYLIBI_H


#include <QtCore/qglobal.h>

#if defined(RTFRAMESHAREDMEMORYLIB_LIBRARY)
#  define RTFRAMESHAREDMEMORYLIBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define RTFRAMESHAREDMEMORYLIBSHARED_EXPORT Q_DECL_IMPORT
#endif

#include <QString>

class RTFRAMESHAREDMEMORYLIBSHARED_EXPORT CRTFrameSharedMemoryLibI
{

public:
    virtual QString GetFrame(QString strChannelName_) = 0;
//    CRTFrameSharedMemoryLibI();
};
extern "C" RTFRAMESHAREDMEMORYLIBSHARED_EXPORT CRTFrameSharedMemoryLibI * CreateDriver();

#endif // CRTFRAMESHAREDMEMORYLIBI_H
