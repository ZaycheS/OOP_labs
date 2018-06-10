#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include "connector.h"
class QTextEdit;
class QLineEdit;
class QPushButton;
class QFile;
class client : public QWidget
{
    Q_OBJECT

public:
    client(const QString& strHost,int nPort,QWidget*pwgt=0);
    bool servermissed=false;
    ~client();
private slots:
    void slotReadyRead();
    void slotSendToServerMessage();
    void slotSendToServerFile();
    void slotSendToServerName();
    /*void enableGetFortuneButton();*/
private:
      connector *builder;
      QString path;
      QTcpSocket* m_pTcpSocket;
      QTextEdit*  m_ptxtInfo;
      QLineEdit*  m_ptxtInput;
      QLineEdit*  m_ptxtInputSend;
      QPushButton* pcmd;
      QPushButton* pcmdf;
      quint64     m_nNextBlockSize;

};
#endif // CLIENT_H
