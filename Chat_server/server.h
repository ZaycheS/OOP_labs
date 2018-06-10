#ifndef CHAT_H
#define CHAT_H
#include <QTcpSocket>
#include <QWidget>
#include "connector.h"
class QLabel;
class QTcpServer;
class QTcpSocket;
class QNetworkSession;
class QTextEdit;
class QLineEdit;
class QPushButton;

class Server : public QWidget
{
    Q_OBJECT

public:
    Server(int nPort,QWidget *pwgt=0);
    bool Client=false;
    ~Server();
private slots:
   virtual void slotNewConnection();
    void slotReadClient();
    void slotSendToClientsMessage();
    void slotSendToClientsFile();
    void slotSaveName();
   void clientDisconnect();



private:
   connector *pSender;
    void sendToClient(QTcpSocket* pSocket, const QString& str);
    void sendToClientFile(QTcpSocket *pSocket,QByteArray data);

QString path;
int Users_count;
    QLabel *statusLabel = nullptr;
    QTcpServer* m_ptcpServer;
    QTextEdit*  m_ptxt;
    QTextEdit *UsersList;
    QLineEdit*  m_ptxtInput;
    QLineEdit*  m_ptxtInputSend;
    quint64     m_nNextBlockSize;
    QTcpSocket * MYarrSockets[10];
    QPushButton* pcmd ;
    QPushButton* pcmdf;
    QString namesarray[10];
    QString nameservuser;
};

#endif // CHAT_H
