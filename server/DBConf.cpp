
#include "dbConf.h"
#include <QtCore>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>


QString HostName = "localhost";
QString UserName = "softeng2";
QString dbPass = "3;ftBE>72h";
QString dbName = "ConclaveApp";
int port = 8889;

bool dbConfigure() {

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setConnectOptions("UNIX_SOCKET=/Applications/MAMP/tmp/mysql/mysql.sock");

    db.setUserName("root");
    db.setPassword("lolloasd92!");
    db.setDatabaseName("ConclaveApp");
    //db.setPort(3306);//8889

    bool ok = db.open();
    qDebug() << ok;
    qDebug() << QCoreApplication::libraryPaths();
    qDebug() << QSqlDatabase::drivers();

    if(db.open()) {
        qDebug() << ok;
        return true;
    }
    else {
        return false;
    }

    if(ok == true)
        return true;
    else
        return false;
}

bool getUserlist(){
     QSqlQuery query;
     query.exec("SELECT username, id FROM User WHERE 1");
     while (query.next()) {
        QString username = query.value(0).toString();
        int id = query.value(1).toInt();
        qDebug() << username << id;
        }
}