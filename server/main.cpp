#include <QtCore/QCoreApplication>
#include "sslechoserver.h"
#include "DBConf.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SslEchoServer server(12345);
    dbConfigure();
    getUserlist();
    checkLoginData("test@test.test", "test");
    Q_UNUSED(server);

    return a.exec();
}