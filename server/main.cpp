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
    //addDocPermission(1,2);
    //qDebug() <<  checkDocPermission(1,1);
    Q_UNUSED(server);

    return a.exec();
}