#include <QtCore/QCoreApplication>
#include "sslechoserver.h"
#include "DBConf.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SslEchoServer server(12345);
    dbConfigure();

    getUserlist();
    //qDebug() <<  createDoc("AAA", 1);
    //addDocPermission(1,2);
    //qDebug() <<  checkDocPermission(1,1);
    // Create invitation code (invURI)
    //QString invURI = createInvite(1);
    //qDebug() <<invURI;
    // Accept the invite for a given code and user id
    //acceptInvite(invURI,2);
    // Check if all went good
    qDebug() <<  checkDocPermission(1,2);
    Q_UNUSED(server);

    return a.exec();
}