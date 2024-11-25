#include "tcpfileserver.h"
#include "tcpfilesender.h"
#include "QTabWidget"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TcpFileServer *w=new TcpFileServer;
    tcpfilesender *q=new tcpfilesender;
    QTabWidget *x=new QTabWidget;
    x->setWindowTitle("test");
    x->addTab(w,"server");
    x->addTab(q,"sender");
    x->show();

    return a.exec();
}
