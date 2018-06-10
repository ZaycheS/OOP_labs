#include "server.h"
#include <QtWidgets>
#include <QtNetwork>
#include <QtCore>


Server::Server(int nPort, QWidget* pwgt /*=0*/)
    : QWidget(pwgt)
    , m_nNextBlockSize(0)
    , Users_count(0)
    ,path("E:\\Downloads\\")
    ,MYarrSockets()
    ,namesarray()
    ,nameservuser("undefined")
{
    m_ptcpServer = new QTcpServer(this);
    if (!m_ptcpServer->listen(QHostAddress::Any,nPort)) {
      //  QMessageBox::critical(0, tr("Server Error"),tr("Unable to start the server: %1.").arg(m_ptcpServer->errorString()));
        Client=true;
        m_ptcpServer->close();
        return;
    }
    pcmd = new QPushButton("&Send");
    connect(pcmd, SIGNAL(clicked()), SLOT(slotSendToClientsMessage()));
    pcmdf = new QPushButton("&Send File");
    connect(pcmdf, SIGNAL(clicked()), SLOT(slotSendToClientsFile()));
    pcmd->setEnabled(false);
     pcmdf->setEnabled(false);
    QPushButton* pcmdg = new QPushButton("&Send Name");
     connect(pcmdg, SIGNAL(clicked()), SLOT(slotSaveName()));
    auto buttonBox=new QDialogButtonBox;
    buttonBox->addButton(pcmd,QDialogButtonBox::ActionRole);
    buttonBox->addButton(pcmdf,QDialogButtonBox::ActionRole);
     buttonBox->addButton(pcmdg,QDialogButtonBox::ActionRole);
     connect(m_ptcpServer,SIGNAL(newConnection()), this, SLOT(slotNewConnection()));
     m_ptxt = new QTextEdit;
     UsersList=new QTextEdit;
     m_ptxtInput=new QLineEdit;
      m_ptxtInputSend=new QLineEdit;
     connect(m_ptxtInput, SIGNAL(returnPressed()),this,SLOT(slotSendToClientsMessage));
         m_ptxt->setReadOnly(true);
         UsersList->setReadOnly(true);
         QVBoxLayout* pvbxLayout = new QVBoxLayout;
            pvbxLayout->addWidget(new QLabel("<H1>Server</H1>"));
            pvbxLayout->addWidget(m_ptxt);
            pvbxLayout->addWidget(UsersList);
            pvbxLayout->addWidget(new QLabel("Send to:"));
            pvbxLayout->addWidget(m_ptxtInputSend);
            pvbxLayout->addWidget(new QLabel("Text:"));
            pvbxLayout->addWidget(m_ptxtInput);
            pvbxLayout->addWidget(buttonBox);
            setLayout(pvbxLayout);
}
void Server::slotSendToClientsMessage()
{
                    QTime   time=QTime::currentTime();
                    QString str=m_ptxtInput->text();
                    m_ptxtInput->setText("");
                    QString sendto=m_ptxtInputSend->text();
                    QString sendername=nameservuser;
                    if(sendto==nameservuser)
                    {
                     QString strMessage =time.toString()+" " +sendername+ ": " + str;
                    m_ptxt->append(strMessage);
                    }
                    else
                    {
                    int k=-1;
                     QString strMessage =time.toString()+" " +sendername+ ": " + str;
                    for(int i=0;i<Users_count;i++)
                    {
                        if(sendto==namesarray[i]) k=i;
                    }
                    if(k==-1)
                    for(int i=0;i<Users_count;i++)
                    {

                        pSender->writeMessage(MYarrSockets[i],strMessage,0);
                    }
                    else
                    {
                       pSender->writeMessage(MYarrSockets[k],strMessage,0);
                    }
                    }
}
void Server::slotSaveName()
{
  nameservuser= m_ptxtInput->text();
   m_ptxtInput->setText("");
  pcmd->setEnabled(true);
  pcmdf->setEnabled(true);
  QPushButton *temp=(QPushButton *)sender();
  temp->setEnabled(false);
  QString strMessage="";
  strMessage.append(nameservuser+"\n");
  for(int i=0;i<Users_count;i++)
  {
    strMessage.append(namesarray[i]+"\n");
  }
  UsersList->setText(strMessage);
  //for(int i=0;i<Users_count;i++)
  {
      for(int i=0;i<Users_count;i++)
      {
        pSender->writeMessage(MYarrSockets[i],strMessage,2);
      }
  }
}
void Server::slotSendToClientsFile()
{
    QFile sendFile(QFileDialog::getOpenFileName(NULL, tr("Upload a file")));
    QByteArray filebyte;
    if(!sendFile.open(QIODevice::ReadOnly))
    {
      return;
    }
    filebyte=sendFile.readAll();
    QFileInfo inf(sendFile);
     QString name=inf.fileName();
    int k=-1;
    for(int i=0;i<Users_count;i++)
    {
        if(m_ptxtInputSend->text()==namesarray[i]) k=i;
    }
    if(k==-1)
    for(int i=0;i<Users_count;i++)
    {
      pSender->writeFile(MYarrSockets[i],name,filebyte,nameservuser);
    }
    else
    {
       pSender->writeFile(MYarrSockets[k],name,filebyte,nameservuser);
    }
    sendFile.close();
}
void Server::slotNewConnection()
{
    QTcpSocket* pClientSocket = m_ptcpServer->nextPendingConnection();
    MYarrSockets[Users_count]=pClientSocket;
    namesarray[Users_count]="undefined";
    Users_count++;
        connect(pClientSocket, SIGNAL(disconnected()),this, SLOT(clientDisconnect()));
        connect(pClientSocket, SIGNAL(readyRead()),this,SLOT(slotReadClient()));
//m_ptxt->append(tr("Current users count:%1").arg(Users_count));
       pSender->writeMessage(pClientSocket,"Server Response: Connected!",0);
}
void Server::clientDisconnect()
{
     QTcpSocket* pClientSocket = (QTcpSocket*)sender();
     pClientSocket->deleteLater();
     bool shift=false;
     for(int i=0;i<Users_count-1;i++)
     {
         if(pClientSocket==MYarrSockets[i]) shift=true;
         if(shift)
         {
             MYarrSockets[i]=MYarrSockets[i+1];
             namesarray[i]=namesarray[i+1];
         }
     }
     Users_count--;
     QString strMessage="";
     strMessage.append(nameservuser+"\n");
     for(int i=0;i<Users_count;i++)
     {
       strMessage.append(namesarray[i]+"\n");
     }
     UsersList->setText(strMessage);
    // for(int i=0;i<Users_count;i++)
     {
         for(int i=0;i<Users_count;i++)
         {
            pSender->writeMessage(MYarrSockets[i],strMessage,2);
         }
     }
    // m_ptxt->append(tr("Current users count:%1").arg(Users_count));
}
void Server::slotReadClient()
{
    QTcpSocket* pClientSocket = (QTcpSocket*)sender();
    QDataStream in(pClientSocket);
    in.setVersion(QDataStream::Qt_4_2);
   int type;
    for (;;) {
        if (!m_nNextBlockSize) {
            if (pClientSocket->bytesAvailable() < sizeof(quint64)) {
               break;
            }
            in >> m_nNextBlockSize;
        }
        if (pClientSocket->bytesAvailable() < m_nNextBlockSize) {

           break;
        }
        in>>type;
        if(type==1)
        {
        QByteArray filebyte;
        QTime time;
        QString name;
        QString sendto;
        in >>time>>sendto>>name>>filebyte;
        QString sendername;
        for(int i=0;i<Users_count;i++)
        {
           if(pClientSocket==MYarrSockets[i])   sendername=namesarray[i];
        }
        if (sendto==nameservuser)
        {
        QFile file(path+name);
           if(file.open(QIODevice::WriteOnly))
        {
             file.write(filebyte);
             file.close();
        }
        QString strMessage =time.toString()+" " +sendername+  tr(" has sended the file:")+name;
        m_ptxt->append(strMessage);
        }
        else
          {
        int k=-1;
        for(int i=0;i<Users_count;i++)
        {
            if(sendto==namesarray[i]) k=i;
        }
        if(k==-1)
        {
            QFile file(path+name);
               if(file.open(QIODevice::WriteOnly))
            {
                 file.write(filebyte);
                 file.close();
            }
            QString strMessage =time.toString()+" " +sendername+  tr(" has sended the file:")+name;
            m_ptxt->append(strMessage);
        for(int i=0;i<Users_count;i++)
        {
                         if(pClientSocket!=MYarrSockets[i])   pSender->writeFile(MYarrSockets[i],name,filebyte,sendername);
        }
        }
        else
        {
           pSender->writeFile(MYarrSockets[k],name,filebyte,sendername);
        }
        m_nNextBlockSize = 0;
          }
        pSender->writeMessage(pClientSocket,"Server Response: Received the file",0);
        }
        else if(type==0)
        {
            QTime   time;
                    QString str;
                    QString sendto;
                    in >> time>>sendto >> str;
                    QString sendername;
                    for(int i=0;i<Users_count;i++)
                    {
                       if(pClientSocket==MYarrSockets[i])   sendername=namesarray[i];
                    }

                    if(sendto==nameservuser)
                    {
                     QString strMessage =time.toString()+" " +sendername+ ": " + str;
                    m_ptxt->append(strMessage);
                    }
                    else
                    {
                    int k=-1;
                     QString strMessage =time.toString()+" " +sendername+ ": " + str;
                    for(int i=0;i<Users_count;i++)
                    {
                        if(sendto==namesarray[i]) k=i;
                    }
                    if(k==-1)
                    {
                    for(int i=0;i<Users_count;i++)
                    {
                      pSender->writeMessage(MYarrSockets[i],strMessage,0);
                    }
                     m_ptxt->append(strMessage);
                    }
                    else
                    {
                       pSender->writeMessage(MYarrSockets[k],strMessage,0);
                    }
                    }

                    m_nNextBlockSize = 0;
        }
        else if(type==2)
        {
            QTime time;
            QString str;
            QString strMessage="";
            in>>time>>str;
             strMessage.append(nameservuser+"\n");
            for(int i=0;i<Users_count;i++)
            {
              if(MYarrSockets[i]==pClientSocket)
              {
                  namesarray[i]=str;

              }
              strMessage.append(namesarray[i]+"\n");
            }
            UsersList->setText(strMessage);
          //  for(int i=0;i<Users_count;i++)
            {
                for(int i=0;i<Users_count;i++)
                {
                   pSender->writeMessage(MYarrSockets[i],strMessage,2);
                }
            }
             m_nNextBlockSize = 0;
        }
}
}
Server::~Server()
{

}
