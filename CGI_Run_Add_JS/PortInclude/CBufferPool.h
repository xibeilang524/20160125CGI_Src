#ifndef CBUFFERPOOL_H
#define CBUFFERPOOL_H

#include <QObject>
#include "typedef.h"
#include "CMyFunction.h"

#define INVALID_VALUE 99

typedef union
{
    bool              m_nBool;
    signed char       m_nChar;
    unsigned char     m_nByte;        //BYTE
    short int         m_nShortInt;
    unsigned short    m_nWord;        //WORD

    int               m_nInt24:24;    //三字节量
    unsigned int      m_nUint24:24;

    int               m_nInt;
    unsigned int      m_nUint;         //UINT
    long  int         m_nLong;
    unsigned long     m_nDWord;        //DWORD
    float             m_nFloat;
    double            m_nDouble;

    BYTE  m_Bytes[8];
} MYVALUE;



class CBufferPool : public QObject
{
    Q_OBJECT
public:
//    explicit CBufferPool(QObject *parent = 0);
    CBufferPool(int nBufferSize_ = 0,QObject *parent_ = 0);
    CBufferPool( void *pDataSource_, int nDataLength_,QObject *parent_ = 0);//构造buffer 同时引用指定内容
    ~CBufferPool();
// Attributes
protected:
    BYTE         *m_pBufferHead;        ///< 缓冲区头指针
    int           m_nBufferSize;        ///< 缓冲区实际大小
    int           m_nInfomationSize;    ///< 缓冲区中信息长度
    bool          m_bRefBuffer;         ///< 是否处于引用Buffer状态
    enum{MAX_BUFFER_SIZE=10240};        ///< 缓冲区最大长度 10k
//attrib operation
public:
    virtual void SetInfoSize(int nNewSize);
    virtual int  GetInfoSize() const;
    virtual int  GetBufferSize()const;
    virtual int  GetEmptySize()const;
    virtual BYTE *GetBuffer(int nStart_=0) const;
    virtual BYTE *GetEmptyBuffer();
    virtual bool IsRefBuffer()const;
public://常用运算符
    operator BYTE *  ();//可以进行类型转换
    BYTE  & operator [](int nOffset_);//可以直接取一单元
//protected:
private:
//    CBufferPool(const CBufferPool &buffer);//{}//无拷贝构造函数
//    CBufferPool & operator =(const CBufferPool &buffer);//{ return *this;}//无等号
//operation
public:
    virtual void SetData(const void *pDataSource_, int nDataLength_, bool bAllocNew_=false);//设置缓冲区内容
    virtual int AppendData(const void *pDataSource_, int nDataLength_);//在缓冲区后增加数据
    virtual int Fifo(int nSize_, int nStart_=0);//进行Fifo操作
    virtual void RefBuffer(void *pData_, int nDataLength_);//引用一缓冲区
    virtual void ClearBuffer();		//删除缓冲区
    virtual int SetBufferSize(int nNewSize_); //设置缓冲区大小
    virtual int ResetBufferSize(int nNewSize_);//改变缓冲区大小,并保持原信息内容不变  nNewSize<=0表示清除缓冲区
    virtual int ResetInfoSize(int nNewSize_);	//改变有效信息长度
    virtual int Move(BYTE *pTarget,int nMoveSize);
    virtual int Copy(void *pTarget, int nCopySize);
    virtual int Compare(int nStartByte,void *pTarget, int nCompareSize)const;
    void AdjustIndex(int &nIndex)const;// 负数表示从数组的后面取  使用中可能出现比较怪的问题

//help
public:
    bool IsValid() const;
signals:

public slots:

};

#endif // CBUFFERPOOL_H
