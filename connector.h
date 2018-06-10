#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QWidget>
#include <QTcpSocket>
#include <QtWidgets>
class connector:public QWidget
{
public:
    connector(const QString& strHost,int nPort,const QString newpath);
    void connectToServer();
    void writeMessage(const QString& str,const QString& sendto);
    void writeFile(const QString& sendto);
    void sendName(const QString& str);
    QString read();
    QTcpSocket* m_pTcpSocket;
private slots:
    /*void slotReadyRead();
    void slotError(QAbstractSocket::SocketError);
    void slotConnected();*/
private:
    QString path;
      quint64     m_nNextBlockSize;
};

#endif // CONNECTOR_H
