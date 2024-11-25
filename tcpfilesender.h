#ifndef TCPFILESENDER_H
#define TCPFILESENDER_H

#include <QDialog>
#include <QtNetwork>
#include <QWidget>
#include <QProgressBar>
#include <QLabel>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLineEdit>

class tcpfilesender : public QDialog
{
    Q_OBJECT

public:
    tcpfilesender(QWidget *parent = nullptr);
    ~tcpfilesender();

public slots:
    void start();
    void startTransfer();
    void updateClientProgress(qint64 numBytes);
    void openFile();

private:
    QLineEdit *ipEdit;
    QLineEdit *portEdit;
    QProgressBar *clientProgressBar;
    QLabel *clientStatusLable;
    QPushButton *startButton;
    QPushButton *quitButton;
    QPushButton *openButton;
    QDialogButtonBox *buttonBox;
    QTcpSocket tcpClient;

    qint64 totalBytes;
    qint64 byteWritten;
    qint64 bytes2Write;
    qint64 loadSize;
    QString fileName;
    QFile *localFile;
    QByteArray outBlock;
};
#endif // TCPFILESENDER_H
