#include "client.h"
#include <QtWidgets>
#include <QtNetwork>

client::client(const QString& strHost,int nPort, QWidget* pwgt/*=0*/)
    : QWidget(pwgt)
    , m_nNextBlockSize(0)

{
builder=new connector(strHost,nPort,"E:\\Downloads\\client\\");
    connect(builder->m_pTcpSocket,SIGNAL(readyRead()),this,SLOT(slotReadyRead()));
        m_ptxtInfo  = new QTextEdit;
        m_ptxtInput = new QLineEdit;
        m_ptxtInputSend = new QLineEdit;
        connect(m_ptxtInput, SIGNAL(returnPressed()),this,SLOT(slotSendToServerMessage));
        m_ptxtInfo->setReadOnly(true);
        pcmd = new QPushButton("&Send");
        connect(pcmd, SIGNAL(clicked()), SLOT(slotSendToServerMessage()));
        pcmdf = new QPushButton("&Send File");
        pcmd->setEnabled(false);
        pcmdf->setEnabled(false);
        connect(pcmdf, SIGNAL(clicked()), SLOT(slotSendToServerFile()));
        QPushButton* pcmdg = new QPushButton("&Send Name");
        connect(pcmdg, SIGNAL(clicked()), SLOT(slotSendToServerName()));
        auto buttonBox=new QDialogButtonBox;
        buttonBox->addButton(pcmd,QDialogButtonBox::ActionRole);
        buttonBox->addButton(pcmdf,QDialogButtonBox::ActionRole);
        buttonBox->addButton(pcmdg,QDialogButtonBox::ActionRole);
        QVBoxLayout* pvbxLayout = new QVBoxLayout;
        pvbxLayout->addWidget(new QLabel("<H1>Chat</H1>"));
        pvbxLayout->addWidget(m_ptxtInfo);
        pvbxLayout->addWidget(new QLabel("Send to:"));
        pvbxLayout->addWidget(m_ptxtInputSend);
         pvbxLayout->addWidget(new QLabel("Text:"));
        pvbxLayout->addWidget(m_ptxtInput);
        pvbxLayout->addWidget(buttonBox);
        setLayout(pvbxLayout);
}
void client::slotSendToServerName()
{
    QPushButton *temp=(QPushButton *)sender();
    pcmd->setEnabled(true);
    pcmdf->setEnabled(true);
    temp->setEnabled(false);
    builder->sendName(m_ptxtInput->text());
     m_ptxtInput->setText("");

}
void client::slotReadyRead()
{
    QString info=builder->read();
    if(info!="") m_ptxtInfo->append(info);
}
void client::slotSendToServerMessage()
{
   /*  QByteArray  arrBlock;
    QDataStream out(&arrBlock, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    int type=0;
    out << quint64(0) <<type<< QTime::currentTime() << str;

    out.device()->seek(0);
    out << quint64(arrBlock.size() - sizeof(quint16));

    pSocket->write(arrBlock);
*/
       builder->writeMessage(m_ptxtInput->text(),m_ptxtInputSend->text());
        m_ptxtInput->setText("");
}
void client::slotSendToServerFile()
{

    builder->writeFile(m_ptxtInputSend->text());
    m_ptxtInput->setText("");

}
client::~client()
{

}
