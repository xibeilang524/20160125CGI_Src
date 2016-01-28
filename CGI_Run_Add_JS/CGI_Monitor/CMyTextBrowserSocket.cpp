/*!
 * \file
 * \brief   概述
 *
 *详细概述
 *
 * \author  zzy
 * \version 版本号
 * \date    2015/4/9
 *
 * \b       修改记录：
 * \li      2015/4/9
 *  添加注释
 */

#include "CMyTextBrowserSocket.h"
#include "Src/cmyfunction.h"

#include <QDebug>
#include <QTcpSocket>
#include <QTimer>
#include <QTime>
#include <QMutex>
#include "CSignFrameDialog.h"


CMyTextBrowserSocket::CMyTextBrowserSocket(CMyTextBrowser *parent) :
    CMyTextBrowser(parent),
    m_pSignFrameDialog(new CSignFrameDialog)
{
    /*!
    * \brief  功能概述 构造函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/4/9
    */
    m_bConnectOk = false;
    m_pTcpSocket = new QTcpSocket;
    connect(m_pTcpSocket,SIGNAL(readyRead()),this,SLOT(ProcessRespond()));
    connect(m_pTcpSocket,SIGNAL(connected()),this,SLOT(OnConnected()));
    connect(m_pTcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));

    connect(this,SIGNAL(SignalSaveFrame(QString)),this,SLOT(SlotSaveFrame(QString)));
    connect(this,SIGNAL(SignalSignFrame()),this,SLOT(SlotSignFrame()));
}

void CMyTextBrowserSocket::closeEvent(QCloseEvent *)
{
    m_pTcpSocket->close();
    delete m_pTcpSocket;
    m_pTcpSocket = NULL;
    emit signalClose(this);
}
void CMyTextBrowserSocket::ProcessRespond()
{
    /*!
    * \brief  功能概述 tcpSocket接收到报文的函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/4/9
    */
    QByteArray datas = m_pTcpSocket->readAll();
    qDebug()<<"monitor:"<<datas.toHex();
    if (datas.isEmpty())
        return;

    if (datas.count()<4)
        return;
    QMutex lock;
    if ((BYTE)datas.at(0) == 0xFF)
    {
        lock.lock();
        DecodeFrame(datas);
        lock.unlock();
    }
}
void CMyTextBrowserSocket::DecodeFrame(QByteArray &datas)
{
    /*!
    * \brief  功能概述 解包函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/4/9
    */
    while(datas.count())
    {
        BYTE szReceive[BUFFER_SIZE];
        const char *pSource = datas.data();
        int nSize = datas.count();
        memcpy(szReceive, pSource, nSize);
        int nLength = szReceive[1]*256+szReceive[2];
        datas.remove(0,nLength);
        BYTE *pFrame = szReceive + 4;
        qDebug()<<"m_nChannelNumber="<<m_nChannelNumber<<nLength<<datas.count()<<"--------";
        if (nLength > nSize)
        {
            return;
        }
        QByteArray array = QByteArray((char *)pFrame,nLength-4);
        switch (szReceive[3]) {
        case 0:
            PrintTXSignFrame(array.toHex(),QColor(0,255,0));
            break;
        case 1:
            PrintRXSignFrame(array.toHex(),QColor(0,0,255));
            break;
        case 2:
            printDebugInfo(array.toHex());
            break;
        default:
            break;
        }
    }
}

int CMyTextBrowserSocket::GetChannelNumber()
{
    /*!
    * \brief  功能概述 获取通道号
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/4/9
    */
    return m_nChannelNumber;
}

//    if (m_pSignFrameDialog->m_pFirstLine->m_isCheck)
//    {
//        if (strBackWord.contains(m_pSignFrameDialog->m_pFirstLine->m_strFrame))
//        {
//            if(m_bTextColor)
//            {
//    //            setTextBackgroundColor(color);
//                setTextBackgroundColor(m_pSignFrameDialog->m_pFirstLine->m_Color);
//                setTextColor(QColor(125,125,125));
//                append(strDateTime);
//                setTextColor(QColor(255,0,0));
//                append(strBackWord);
//            }
//            else
//            {
//                setTextBackgroundColor(QColor(255,255,255));
//                setTextColor(QColor(125,125,125));
//                append(strDateTime);
//    //            setTextColor(color);
//                setTextColor(m_pSignFrameDialog->m_pFirstLine->m_Color);
//                append(strBackWord);
//            }
//            return;
//        }else
//        {
//            qDebug()<<m_pSignFrameDialog->m_pFirstLine->m_strFrame;
//        }
//    }
//    if (m_pSignFrameDialog->m_pSecondLine->m_isCheck)
//    {
//        if (strBackWord.contains(m_pSignFrameDialog->m_pSecondLine->m_strFrame))
//        {
//            if(m_bTextColor)
//            {
//    //            setTextBackgroundColor(color);
//                setTextBackgroundColor(m_pSignFrameDialog->m_pSecondLine->m_Color);
//                setTextColor(QColor(125,125,125));
//                append(strDateTime);
//                setTextColor(QColor(255,0,0));
//                append(strBackWord);
//            }
//            else
//            {
//                setTextBackgroundColor(QColor(255,255,255));
//                setTextColor(QColor(125,125,125));
//                append(strDateTime);
//    //            setTextColor(color);
//                setTextColor(m_pSignFrameDialog->m_pSecondLine->m_Color);
//                append(strBackWord);
//            }
//            return;
//        }
//    }
//    if (m_pSignFrameDialog->m_pThirdLine->m_isCheck)
//    {
//        if (strBackWord.contains(m_pSignFrameDialog->m_pThirdLine->m_strFrame))
//        {
//            if(m_bTextColor)
//            {
//    //            setTextBackgroundColor(color);
//                setTextBackgroundColor(m_pSignFrameDialog->m_pThirdLine->m_Color);
//                setTextColor(QColor(125,125,125));
//                append(strDateTime);
//                setTextColor(QColor(255,0,0));
//                append(strBackWord);
//            }
//            else
//            {
//                setTextBackgroundColor(QColor(255,255,255));
//                setTextColor(QColor(125,125,125));
//                append(strDateTime);
//    //            setTextColor(color);
//                setTextColor(m_pSignFrameDialog->m_pThirdLine->m_Color);
//                append(strBackWord);
//            }
//            return;
//        }
//    }
//    if (m_pSignFrameDialog->m_pFourthLine->m_isCheck)
//    {
//        if (strBackWord.contains(m_pSignFrameDialog->m_pFourthLine->m_strFrame))
//        {
//            if(m_bTextColor)
//            {
//    //            setTextBackgroundColor(color);
//                setTextBackgroundColor(m_pSignFrameDialog->m_pFourthLine->m_Color);
//                setTextColor(QColor(125,125,125));
//                append(strDateTime);
//                setTextColor(QColor(255,0,0));
//                append(strBackWord);
//            }
//            else
//            {
//                setTextBackgroundColor(QColor(255,255,255));
//                setTextColor(QColor(125,125,125));
//                append(strDateTime);
//    //            setTextColor(color);
//                setTextColor(m_pSignFrameDialog->m_pFourthLine->m_Color);
//                append(strBackWord);
//            }
//            return;
//        }
//    }
//    if (m_pSignFrameDialog->m_pFifthLine->m_isCheck)
//    {
//        if (strBackWord.contains(m_pSignFrameDialog->m_pFifthLine->m_strFrame))
//        {
//            if(m_bTextColor)
//            {
//    //            setTextBackgroundColor(color);
//                setTextBackgroundColor(m_pSignFrameDialog->m_pFifthLine->m_Color);
//                setTextColor(QColor(125,125,125));
//                append(strDateTime);
//                setTextColor(QColor(255,0,0));
//                append(strBackWord);
//            }
//            else
//            {
//                setTextBackgroundColor(QColor(255,255,255));
//                setTextColor(QColor(125,125,125));
//                append(strDateTime);
//    //            setTextColor(color);
//                setTextColor(m_pSignFrameDialog->m_pFifthLine->m_Color);
//                append(strBackWord);
//            }
//            return;
//        }
//    }
//    if (m_pSignFrameDialog->m_pSixthLine->m_isCheck)
//    {
//        if (strBackWord.contains(m_pSignFrameDialog->m_pSixthLine->m_strFrame))
//        {
//            if(m_bTextColor)
//            {
//    //            setTextBackgroundColor(color);
//                setTextBackgroundColor(m_pSignFrameDialog->m_pSixthLine->m_Color);
//                setTextColor(QColor(125,125,125));
//                append(strDateTime);
//                setTextColor(QColor(255,0,0));
//                append(strBackWord);
//            }
//            else
//            {
//                setTextBackgroundColor(QColor(255,255,255));
//                setTextColor(QColor(125,125,125));
//                append(strDateTime);
//    //            setTextColor(color);
//                setTextColor(m_pSignFrameDialog->m_pSixthLine->m_Color);
//                append(strBackWord);
//            }
//            return;
//        }
//    }
void CMyTextBrowserSocket::PrintTXSignFrame(QString str, QColor color)
{
    /*!
    * \brief  功能概述 打印发送报文
    * \param  参数描述 str打印的信息
    * \return 返回值描述 无
    * \author zzy
    * \date   2015/1/16
    */
    str.toUpper();///小写转大写
//    nClearCounter++;
//    if(nClearCounter > 512)
//    {
//        nClearCounter = 0;
//        clear();
//    }
    ////////保存报文////////
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz");
    QString strBackWord = "发送 --->: ";
    strBackWord += str;
    if (m_bSaveFrame)
    {
        emit SignalSaveFrame(strDateTime+" "+strBackWord);
    }
    if(getPauseFlag())
        return;

    if(m_bTextColor)
    {
        setTextBackgroundColor(color);
        setTextColor(QColor(125,125,125));
        append(strDateTime);
        setTextColor(QColor(255,0,0));
        append(strBackWord);
    }
    else
    {
        setTextBackgroundColor(QColor(255,255,255));
        setTextColor(QColor(125,125,125));
        append(strDateTime);
        setTextColor(color);
        append(strBackWord);
    }
    SetTextFormat();
}

void CMyTextBrowserSocket::PrintRXSignFrame(QString str, QColor color)
{
    /*!
    * \brief  功能概述 打印接收报文
    * \param  参数描述 str打印的信息
    * \return 返回值描述 无
    * \author zzy
    * \date   2015/1/16
    */
    str.toUpper();///小写转大写
//    nClearCounter++;
//    if(nClearCounter > 512)
//    {
//        nClearCounter = 0;
//        clear();
//    }
    ////////保存报文////////
//    if(m_bSaveMsgFlag) SaveMsg(str,1);
    QString strDateTime = QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss.zzz");
    QString strForWord = "接收 <---: ";
    strForWord += str;
    if (m_bSaveFrame)
    {
        emit SignalSaveFrame(strDateTime+" "+strForWord);
    }
    if(getPauseFlag())
        return;

    if(m_bTextColor)
    {
        setTextBackgroundColor(color);
        setTextColor(QColor(125,125,125));
        append(strDateTime);
        setTextColor(QColor(255,0,0));
        append(strForWord);
    }
    else
    {
        setTextBackgroundColor(QColor(255,255,255));
        setTextColor(QColor(125,125,125));
        append(strDateTime);
        setTextColor(color);
        append(strForWord);
    }
    SetTextFormat();
}

void CMyTextBrowserSocket::SetTextFormat()
{
    /*!
    * \brief  功能概述 改变特殊报文的显示颜色
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/4/9
    */
    if (m_pSignFrameDialog->m_pFirstLine->m_isCheck)
    {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition( QTextCursor::Start );//文档开始
        int strCount = m_pSignFrameDialog->m_pFirstLine->m_strFrame.count();
        if (strCount <= 0)
            return;
        while (!this->document()->find(m_pSignFrameDialog->m_pFirstLine->m_strFrame,cursor).isNull())
        {
            qDebug()<<"非空"<<m_pSignFrameDialog->m_pFirstLine->m_strFrame;
            cursor = this->document()->find(m_pSignFrameDialog->m_pFirstLine->m_strFrame,cursor);
//            cursor.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor, strCount-1);//向右移动到Pos
            cursor.movePosition( QTextCursor::NoMove, QTextCursor::MoveAnchor, strCount-1);//向右移动到Pos
//            cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor );
            cursor.movePosition( QTextCursor::NoMove, QTextCursor::KeepAnchor );
            this->setTextCursor(cursor);
            QTextCharFormat defcharfmt = this->currentCharFormat();
            QTextCharFormat newcharfmt = defcharfmt;
//            newcharfmt.setFontUnderline(true);
//            newcharfmt.setUnderlineColor(m_pSignFrameDialog->m_pFirstLine->m_Color);
//            newcharfmt.setUnderlineStyle( QTextCharFormat::SingleUnderline );

//            newcharfmt.setBackground(QColor("#EEEE00"));
//            newcharfmt.setBackground(m_pSignFrameDialog->m_pFirstLine->m_Color);
            newcharfmt.setForeground(m_pSignFrameDialog->m_pFirstLine->m_Color);
//            newcharfmt.setFontPointSize(fontSize);
//            newcharfmt.setFontWeight(QFont::Bold);
//            highlightedFormat.setBackground(Qt::yellow);
//            newcharfmt.setForeground(Qt::red);


            this->setCurrentCharFormat(newcharfmt);

            cursor.movePosition( QTextCursor::PreviousCharacter );//加上这句是为了去除光标selected
            this->setTextCursor( cursor ); // added
            this->setFocus();
            cursor.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor,strCount);//向右移动到Pos
            if (cursor.isNull())
                break;
        }
    }

    if (m_pSignFrameDialog->m_pSecondLine->m_isCheck)
    {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition( QTextCursor::Start );//文档开始
        int strCount = m_pSignFrameDialog->m_pSecondLine->m_strFrame.count();
        if (strCount <= 0)
            return;
        while (!this->document()->find(m_pSignFrameDialog->m_pSecondLine->m_strFrame,cursor).isNull())
        {
            qDebug()<<"非空"<<m_pSignFrameDialog->m_pSecondLine->m_strFrame;
            cursor = this->document()->find(m_pSignFrameDialog->m_pSecondLine->m_strFrame,cursor);
//            cursor.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor, strCount-1);//向右移动到Pos
            cursor.movePosition( QTextCursor::NoMove, QTextCursor::MoveAnchor, strCount-1);//向右移动到Pos
//            cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor );
            cursor.movePosition( QTextCursor::NoMove, QTextCursor::KeepAnchor );
            this->setTextCursor(cursor);
            QTextCharFormat defcharfmt = this->currentCharFormat();
            QTextCharFormat newcharfmt = defcharfmt;
//            newcharfmt.setFontUnderline(true);
//            newcharfmt.setUnderlineColor(m_pSignFrameDialog->m_pFirstLine->m_Color);
//            newcharfmt.setUnderlineStyle( QTextCharFormat::SingleUnderline );

//            newcharfmt.setBackground(QColor("#EEEE00"));
//            newcharfmt.setBackground(m_pSignFrameDialog->m_pFirstLine->m_Color);
            newcharfmt.setForeground(m_pSignFrameDialog->m_pSecondLine->m_Color);
//            newcharfmt.setFontPointSize(fontSize);
//            newcharfmt.setFontWeight(QFont::Bold);
//            highlightedFormat.setBackground(Qt::yellow);
//            newcharfmt.setForeground(Qt::red);


            this->setCurrentCharFormat(newcharfmt);

            cursor.movePosition( QTextCursor::PreviousCharacter );//加上这句是为了去除光标selected
            this->setTextCursor( cursor ); // added
            this->setFocus();
            cursor.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor,strCount);//向右移动到Pos
            if (cursor.isNull())
                break;
        }
    }
    if (m_pSignFrameDialog->m_pThirdLine->m_isCheck)
    {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition( QTextCursor::Start );//文档开始
        int strCount = m_pSignFrameDialog->m_pThirdLine->m_strFrame.count();
        if (strCount <= 0)
            return;
        while (!this->document()->find(m_pSignFrameDialog->m_pThirdLine->m_strFrame,cursor).isNull())
        {
            qDebug()<<"非空"<<m_pSignFrameDialog->m_pThirdLine->m_strFrame;
            cursor = this->document()->find(m_pSignFrameDialog->m_pThirdLine->m_strFrame,cursor);
//            cursor.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor, strCount-1);//向右移动到Pos
            cursor.movePosition( QTextCursor::NoMove, QTextCursor::MoveAnchor, strCount-1);//向右移动到Pos
//            cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor );
            cursor.movePosition( QTextCursor::NoMove, QTextCursor::KeepAnchor );
            this->setTextCursor(cursor);
            QTextCharFormat defcharfmt = this->currentCharFormat();
            QTextCharFormat newcharfmt = defcharfmt;
//            newcharfmt.setFontUnderline(true);
//            newcharfmt.setUnderlineColor(m_pSignFrameDialog->m_pFirstLine->m_Color);
//            newcharfmt.setUnderlineStyle( QTextCharFormat::SingleUnderline );

//            newcharfmt.setBackground(QColor("#EEEE00"));
//            newcharfmt.setBackground(m_pSignFrameDialog->m_pFirstLine->m_Color);
            newcharfmt.setForeground(m_pSignFrameDialog->m_pThirdLine->m_Color);
//            newcharfmt.setFontPointSize(fontSize);
//            newcharfmt.setFontWeight(QFont::Bold);
//            highlightedFormat.setBackground(Qt::yellow);
//            newcharfmt.setForeground(Qt::red);


            this->setCurrentCharFormat(newcharfmt);

            cursor.movePosition( QTextCursor::PreviousCharacter );//加上这句是为了去除光标selected
            this->setTextCursor( cursor ); // added
            this->setFocus();
            cursor.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor,strCount);//向右移动到Pos
            if (cursor.isNull())
                break;
        }
    }
    if (m_pSignFrameDialog->m_pFourthLine->m_isCheck)
    {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition( QTextCursor::Start );//文档开始
        int strCount = m_pSignFrameDialog->m_pFourthLine->m_strFrame.count();
        if (strCount <= 0)
            return;
        while (!this->document()->find(m_pSignFrameDialog->m_pFourthLine->m_strFrame,cursor).isNull())
        {
            qDebug()<<"非空"<<m_pSignFrameDialog->m_pFourthLine->m_strFrame;
            cursor = this->document()->find(m_pSignFrameDialog->m_pFourthLine->m_strFrame,cursor);
//            cursor.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor, strCount-1);//向右移动到Pos
            cursor.movePosition( QTextCursor::NoMove, QTextCursor::MoveAnchor, strCount-1);//向右移动到Pos
//            cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor );
            cursor.movePosition( QTextCursor::NoMove, QTextCursor::KeepAnchor );
            this->setTextCursor(cursor);
            QTextCharFormat defcharfmt = this->currentCharFormat();
            QTextCharFormat newcharfmt = defcharfmt;
//            newcharfmt.setFontUnderline(true);
//            newcharfmt.setUnderlineColor(m_pSignFrameDialog->m_pFirstLine->m_Color);
//            newcharfmt.setUnderlineStyle( QTextCharFormat::SingleUnderline );

//            newcharfmt.setBackground(QColor("#EEEE00"));
//            newcharfmt.setBackground(m_pSignFrameDialog->m_pFirstLine->m_Color);
            newcharfmt.setForeground(m_pSignFrameDialog->m_pFourthLine->m_Color);
//            newcharfmt.setFontPointSize(fontSize);
//            newcharfmt.setFontWeight(QFont::Bold);
//            highlightedFormat.setBackground(Qt::yellow);
//            newcharfmt.setForeground(Qt::red);


            this->setCurrentCharFormat(newcharfmt);

            cursor.movePosition( QTextCursor::PreviousCharacter );//加上这句是为了去除光标selected
            this->setTextCursor( cursor ); // added
            this->setFocus();
            cursor.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor,strCount);//向右移动到Pos
            if (cursor.isNull())
                break;
        }
    }
    if (m_pSignFrameDialog->m_pFifthLine->m_isCheck)
    {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition( QTextCursor::Start );//文档开始
        int strCount = m_pSignFrameDialog->m_pFifthLine->m_strFrame.count();
        if (strCount <= 0)
            return;
        while (!this->document()->find(m_pSignFrameDialog->m_pFifthLine->m_strFrame,cursor).isNull())
        {
            qDebug()<<"非空"<<m_pSignFrameDialog->m_pFifthLine->m_strFrame;
            cursor = this->document()->find(m_pSignFrameDialog->m_pFifthLine->m_strFrame,cursor);
//            cursor.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor, strCount-1);//向右移动到Pos
            cursor.movePosition( QTextCursor::NoMove, QTextCursor::MoveAnchor, strCount-1);//向右移动到Pos
//            cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor );
            cursor.movePosition( QTextCursor::NoMove, QTextCursor::KeepAnchor );
            this->setTextCursor(cursor);
            QTextCharFormat defcharfmt = this->currentCharFormat();
            QTextCharFormat newcharfmt = defcharfmt;
//            newcharfmt.setFontUnderline(true);
//            newcharfmt.setUnderlineColor(m_pSignFrameDialog->m_pFirstLine->m_Color);
//            newcharfmt.setUnderlineStyle( QTextCharFormat::SingleUnderline );

//            newcharfmt.setBackground(QColor("#EEEE00"));
//            newcharfmt.setBackground(m_pSignFrameDialog->m_pFirstLine->m_Color);
            newcharfmt.setForeground(m_pSignFrameDialog->m_pFifthLine->m_Color);
//            newcharfmt.setFontPointSize(fontSize);
//            newcharfmt.setFontWeight(QFont::Bold);
//            highlightedFormat.setBackground(Qt::yellow);
//            newcharfmt.setForeground(Qt::red);


            this->setCurrentCharFormat(newcharfmt);

            cursor.movePosition( QTextCursor::PreviousCharacter );//加上这句是为了去除光标selected
            this->setTextCursor( cursor ); // added
            this->setFocus();
            cursor.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor,strCount);//向右移动到Pos
            if (cursor.isNull())
                break;
        }
    }
    if (m_pSignFrameDialog->m_pSixthLine->m_isCheck)
    {
        QTextCursor cursor = this->textCursor();
        cursor.movePosition( QTextCursor::Start );//文档开始
        int strCount = m_pSignFrameDialog->m_pSixthLine->m_strFrame.count();
        if (strCount <= 0)
            return;
        while (!this->document()->find(m_pSignFrameDialog->m_pSixthLine->m_strFrame,cursor).isNull())
        {
            qDebug()<<"非空"<<m_pSignFrameDialog->m_pSixthLine->m_strFrame;
            cursor = this->document()->find(m_pSignFrameDialog->m_pSixthLine->m_strFrame,cursor);
//            cursor.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor, strCount-1);//向右移动到Pos
            cursor.movePosition( QTextCursor::NoMove, QTextCursor::MoveAnchor, strCount-1);//向右移动到Pos
//            cursor.movePosition( QTextCursor::NextCharacter, QTextCursor::KeepAnchor );
            cursor.movePosition( QTextCursor::NoMove, QTextCursor::KeepAnchor );
            this->setTextCursor(cursor);
            QTextCharFormat defcharfmt = this->currentCharFormat();
            QTextCharFormat newcharfmt = defcharfmt;
//            newcharfmt.setFontUnderline(true);
//            newcharfmt.setUnderlineColor(m_pSignFrameDialog->m_pFirstLine->m_Color);
//            newcharfmt.setUnderlineStyle( QTextCharFormat::SingleUnderline );

//            newcharfmt.setBackground(QColor("#EEEE00"));
//            newcharfmt.setBackground(m_pSignFrameDialog->m_pFirstLine->m_Color);
            newcharfmt.setForeground(m_pSignFrameDialog->m_pSixthLine->m_Color);
//            newcharfmt.setFontPointSize(fontSize);
//            newcharfmt.setFontWeight(QFont::Bold);
//            highlightedFormat.setBackground(Qt::yellow);
//            newcharfmt.setForeground(Qt::red);


            this->setCurrentCharFormat(newcharfmt);

            cursor.movePosition( QTextCursor::PreviousCharacter );//加上这句是为了去除光标selected
            this->setTextCursor( cursor ); // added
            this->setFocus();
            cursor.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor,strCount);//向右移动到Pos
            if (cursor.isNull())
                break;
        }
    }
}

void CMyTextBrowserSocket::SlotSaveFrame(QString str)
{
    /*!
    * \brief  功能概述 保存报文,将报文写入文件
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/4/9
    */
    if (m_SaveFrameFile.isOpen())
    {
        m_TextStreamOut.setDevice(&m_SaveFrameFile);

        m_TextStreamOut<<str<<'\n';
        m_SaveFrameFile.flush();
    }else
    {
        m_SaveFrameFile.setFileName(m_strWindowTitle+".log");
        if (m_SaveFrameFile.open(QIODevice::ReadWrite|QIODevice::Text|QIODevice::Append))
        {
            qDebug()<<"生成文件了"<<m_strWindowTitle+".log";
        }else
        {
            qDebug()<<"未生成文件了"<<m_strWindowTitle+".log";

        }
    }
}

void CMyTextBrowserSocket::SlotSignFrame()
{
    /*!
    * \brief  功能概述 特殊报文显示槽函数
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/4/9
    */
    m_pSignFrameDialog->show();
//    m_pSignFrameDialog->exec();/// @note 弹出设置固定字符串改变颜色的对话框
}

void CMyTextBrowserSocket::Start()
{
    this->setGeometry(100,100,1000,800);
    this->setWindowTitle(m_strWindowTitle+" 报文显示窗口");
    this->show();
    OnOpenSocket();
}
void CMyTextBrowserSocket::OnConnected()
{
    /*!
    * \brief  功能概述 连接成功
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/17
    */
    m_bConnectOk = true;///
    qDebug()<<"MonitorRun:connect succeed."<<CMyFunction::MonitorIPAddress<<FramePort;
    SendStartFrame();/// @note 发送启动帧
//    m_nProtcolTimerId = startTimer(50);
}
void CMyTextBrowserSocket::displayError(QAbstractSocket::SocketError)
{
    /*!
    * \brief  功能概述 未连接服务器,重新连接
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/17
    */
    qDebug()<<"MonitorRun:"<<m_pTcpSocket->errorString()<<"Tcp connect failed"<<CMyFunction::MonitorIPAddress<<FramePort<<QTime::currentTime().toString("hh:mm:ss.zzz");
    m_pTcpSocket->close();
    m_bConnectOk = false;
    QTimer::singleShot(1000,this,SLOT(OnOpenSocket()));
}
void CMyTextBrowserSocket::OnOpenSocket()
{
    /*!
    * \brief  功能概述 连接服务器
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/3/17
    */
    qDebug()<<"MonitorRun:connect TCP Server"<<CMyFunction::MonitorIPAddress<<FramePort<<QTime::currentTime().toString("hh:mm:ss.zzz");
    m_pTcpSocket->connectToHost(CMyFunction::MonitorIPAddress,FramePort);

}
bool CMyTextBrowserSocket::SendStartFrame()
{
    /*!
    * \brief  功能概述 合成启动报文 0xFF报文头
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/4/9
    */
    BYTE szSend[BUFFER_SIZE] = {0};
    BYTE *pCur = szSend;

    *pCur++ = 0xFF;/// @note 报文头
    *pCur++ = m_nChannelNumber;/// @note 通道号

    return SetData(szSend,pCur-szSend);
}

bool CMyTextBrowserSocket::SetData(const BYTE *pSource_, int nLength_)
{
    /*!
    * \brief  功能概述 发送报文
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/4/9
    */
    QByteArray ByteArrayFrame = QByteArray((const char*)pSource_,nLength_);
    if (m_pTcpSocket == NULL)
    {
        return false;
    }
    if (m_pTcpSocket->isOpen())
    {
        m_pTcpSocket->write(ByteArrayFrame);
        return true;
    }
    return false;
}
void CMyTextBrowserSocket::SetChannelMessage(int nChannelNumber_, const QString &strWindowTitle_)
{
    /*!
    * \brief  功能概述 初始化通道号和通道名称
    * \param  参数描述
    * \return 返回值描述
    * \author zzy
    * \date   2015/4/9
    */
    m_nChannelNumber = nChannelNumber_;
    m_strWindowTitle = strWindowTitle_;
}

