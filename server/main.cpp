#include <QtCore/QCoreApplication>
#include "sslechoserver.h"
#include "DBConf.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SslEchoServer server(12345);
    dbConfigure();
    getUserlist();

    Q_UNUSED(server);

    return a.exec();
}