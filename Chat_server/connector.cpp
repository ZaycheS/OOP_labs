#include "connector.h"

connector::connector(const QString& strHost, int nPort, const QString newpath)
 :m_nNextBlockSize(0)
{
    path=newpath;
   /* connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotError(QAbstractSocket::SocketError)));*/

}
void connector::writeMessage(QTcpSocket *pSocket,const QString &str,int type)
{
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint64(0) <<type<< QTime::currentTime() << str;

    out.device()->seek(0);
    out << quint64(arrBlock.size() - sizeof(quint64));

    pSocket->write(arrBlock);
}
void connector::writeFile(QTcpSocket*pSocket,const QString &name,QByteArray filebyte,const QString &sender)
{
    int type=1;
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint64(0) ;
   out<<type<<QTime::currentTime()<<name<<filebyte<<sender;
    out.device()->seek(0);
    out << quint64(arrBlock.size() - sizeof(quint64));
    pSocket->write(arrBlock);
}
