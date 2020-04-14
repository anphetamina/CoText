/*
#include "dbConf.h"
#include <QtCore>
#include <QMessageBox>
#include <QDebug>


QString HostName = "localhost";
QString UserName = "softeng2";
QString dbPass = "3;ftBE>72h";
QString dbName = "ConclaveApp";
int port = 8889;

bool dbConfigure() {

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("root");
    db.setDatabaseName("ConclaveApp");
    db.setPort(8889);

    bool ok = db.open();
    qDebug()<<ok;
    qDebug()<< QCoreApplication::libraryPaths();
    qDebug()<<QSqlDatabase::drivers();



    if(db.open())
        qDebug()<<ok;
        return true;
    else  //db not correctly opened
        return false;

    if(ok == true)
        return true;
    else
        return false;
}
*/
