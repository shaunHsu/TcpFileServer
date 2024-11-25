#include "tcpfilesender.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

tcpfilesender::tcpfilesender(QWidget *parent)
    : QDialog(parent)
{    loadSize=1024*4;
    totalBytes=0;
    byteWritten=0;
    bytes2Write=0;
    QLabel *ipLabel=new QLabel(tr("ip    "));
    ipEdit=new QLineEdit;
    ipEdit->setText("127.0.0.1");
    QLabel *portLabel=new QLabel(tr("port"));
    portEdit=new QLineEdit;
    portEdit->setText("16998");
    clientProgressBar=new QProgressBar;
    clientStatusLable=new QLabel(tr("客戶端就緒"));
    startButton=new QPushButton(tr("開始"));
    quitButton=new QPushButton(tr("退出"));
    openButton=new QPushButton(tr("開檔"));
    startButton->setEnabled(false);
    buttonBox=new QDialogButtonBox;
    buttonBox->addButton(startButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton,QDialogButtonBox::ActionRole);
    buttonBox->addButton(openButton,QDialogButtonBox::ActionRole);

    QHBoxLayout *ipLayout=new QHBoxLayout;
    ipLayout->addWidget(ipLabel);
    ipLayout->addWidget(ipEdit);
    QHBoxLayout *portLayout=new QHBoxLayout;
    portLayout->addWidget(portLabel);
    portLayout->addWidget(portEdit);

    QVBoxLayout *mainWindow=new QVBoxLayout;
    mainWindow->addLayout(ipLayout);
    mainWindow->addLayout(portLayout);
    mainWindow->addWidget(clientProgressBar);
    mainWindow->addWidget(clientStatusLable);
    mainWindow->addStretch(1);
    mainWindow->addSpacing(10);
    mainWindow->addWidget(buttonBox);
    setLayout(mainWindow);
    setWindowTitle(tr("檔案傳送"));
    connect(openButton,SIGNAL(clicked()),this,SLOT(openFile()));
    connect(quitButton,SIGNAL(clicked()),this,SLOT(close()));
    connect(startButton,SIGNAL(clicked()),this,SLOT(start()));
    connect(&tcpClient,SIGNAL(connected()),this,SLOT(startTransfer()));
    connect(&tcpClient,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));
}

tcpfilesender::~tcpfilesender() {}

void tcpfilesender::start(){
    startButton->setEnabled(false);
    byteWritten=0;
    clientStatusLable->setText(tr("連接中..."));
    QString ip=ipEdit->text();
    if(ip.isEmpty())ip="127.0.0.1";
    quint16 port=portEdit->text().toUShort();
    tcpClient.connectToHost(ip,port);
}
void tcpfilesender::startTransfer(){
    localFile=new QFile(fileName);
    if(!localFile->open(QFile::ReadOnly)){
        QMessageBox::warning(this,tr("應用程式"),tr("無法讀取 %1:\n%2").arg(fileName).arg(localFile->errorString()));
        return;
    }
    totalBytes=localFile->size();
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_4_6);
    QString currentFile=fileName.right(fileName.size()-fileName.lastIndexOf("/")-1);
    sendOut<<qint64(0)<<qint64(0)<<currentFile;
    totalBytes+=outBlock.size();

    sendOut.device()->seek(0);
    sendOut<<totalBytes<<qint64((outBlock.size()-sizeof(qint64)*2));
    bytes2Write=totalBytes-tcpClient.write(outBlock);
    clientStatusLable->setText(tr("已連接"));
    qDebug()<<currentFile<<totalBytes;
    outBlock.resize(0);
}
void tcpfilesender::updateClientProgress(qint64 numBytes){
    byteWritten+=(int)numBytes;
    if(bytes2Write>0){
        outBlock=localFile->read(qMin(bytes2Write,loadSize));
        bytes2Write-=(int)tcpClient.write(outBlock);
        outBlock.resize(0);
    }
    else{
        localFile->close();
    }
    clientProgressBar->setMaximum(totalBytes);
    clientProgressBar->setValue(byteWritten);
    clientStatusLable->setText(tr("已傳送 %1 Byte").arg(byteWritten));
}
void tcpfilesender::openFile(){
    fileName=QFileDialog::getOpenFileName(this);
    if(!fileName.isEmpty())startButton->setEnabled(true);
}
