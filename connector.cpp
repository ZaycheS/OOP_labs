#include "connector.h"

connector::connector(const QString& strHost, int nPort, const QString newpath)
 :m_nNextBlockSize(0)
{
    m_pTcpSocket=new QTcpSocket(this);
    m_pTcpSocket->connectToHost(strHost, nPort);
    path=newpath;
   /* connect(m_pTcpSocket, SIGNAL(connected()), SLOT(slotConnected()));
    connect(m_pTcpSocket, SIGNAL(readyRead()), SLOT(slotReadyRead()));
    connect(m_pTcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotError(QAbstractSocket::SocketError)));*/

}
void connector::writeMessage(const QString &str, const QString &sendto)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
  int type=0;
    out << quint64(0) <<type<< QTime::currentTime()<<sendto<< str;
    out.device()->seek(0);
    out << quint64(arrBlock.size() - sizeof(quint64));
    m_pTcpSocket->write(arrBlock);
}
void connector::sendName(const QString &str)
{
    QByteArray arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
  int type=2;
    out << quint64(0) <<type<< QTime::currentTime()<< str;
    out.device()->seek(0);
    out << quint64(arrBlock.size() - sizeof(quint64));
    m_pTcpSocket->write(arrBlock);
}
void connector::writeFile(const QString &sendto)
{
    QFile sendFile(QFileDialog::getOpenFileName(NULL, tr("Upload a file")));
    QByteArray filebyte;
    if(!sendFile.open(QIODevice::ReadOnly))
    {
      return;
    }
    int type=1;
     filebyte=sendFile.readAll();
     QFileInfo inf(sendFile);
     QString name=inf.fileName();
    QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << quint64(0) ;
   out<<type<<QTime::currentTime()<<sendto<<name<<filebyte;
    out.device()->seek(0);
    out << quint64(arrBlock.size() - sizeof(quint64));
    m_pTcpSocket->write(arrBlock);
 // m_ptxtInfo->append(tr("\nSIZE:%1").arg(filebyte.size()));
  filebyte.resize(0);
    sendFile.close();
}
QString connector::read()
{
    QDataStream in(m_pTcpSocket);
    in.setVersion(QDataStream::Qt_4_2);
    QString strMessage;
    int type;
    for (;;) {
        if (!m_nNextBlockSize) {
            if (m_pTcpSocket->bytesAvailable() < sizeof(quint64)) {
                break;
            }
            in >> m_nNextBlockSize;
        }

        if (m_pTcpSocket->bytesAvailable() < m_nNextBlockSize) {
            break;
        }

        in>>type;
        if(type==1)
        {
            QByteArray filebyte;
            QTime time;
            QString name;
            QString sendername;
            in >>time>>name>>filebyte>>sendername;
            QFile file(path+name);
            if(file.open(QIODevice::WriteOnly))
            {
                 file.write(filebyte);
                 file.close();
            }
            strMessage =time.toString()+" " +sendername + " has sended the file:"+name;
            m_nNextBlockSize = 0;
        }
        else if(type==0)
        {
        QTime   time;
        QString str;
        in >> time >> str;
        m_nNextBlockSize = 0;
       strMessage=str;
        }
         else if(type==2)
        {
            QTime   time;
            QString str;
            in >> time >> str;
            strMessage="New users list:\n"+str;
            m_nNextBlockSize = 0;

        }
    }
   return(strMessage);
}
