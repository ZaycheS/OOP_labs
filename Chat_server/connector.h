#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QWidget>
#include <QTcpSocket>
#include <QtWidgets>
class connector:public QWidget
{
public:
    connector(const QString& strHost,int nPort,const QString newpath);
    void writeMessage(QTcpSocket *pSocket,const QString &str,int type);
    void writeFile(QTcpSocket*pSocket,const QString &name,QByteArray filebyte,const QString &sender);
    void sendName(const QString& str);
    QString read();
private slots:
    /*void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotConnected();*/
private:
    QString path;
      quint64     m_nNextBlockSize;
};

#endif // CONNECTOR_H
