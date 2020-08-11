
#include "dbConf.h"
#include "User.h"
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

User checkLoginData(QString email, QString password){
    QSqlQuery query;
    QString hashedpassword = password;
    // Avoiding security concerns about sql for now
    QString squery = "SELECT username, id, email, name, surname FROM User WHERE email='"+email+"' AND password='" + hashedpassword + "'";
    //qDebug() << "[DB] Composed query to be executed: " << squery;
    query.exec(squery);

    if (query.next()) {
        QString username = query.value(0).toString();
        int id = query.value(1).toInt();
        QString email = query.value(2).toString();
        QString name = query.value(3).toString();
        QString surname = query.value(4).toString();
        User loggedUser = User(id, email, name, surname);
        qDebug() << "[AUTH] New user authenticated with success. Email: " << loggedUser.getEmail() << endl << " Name:" << loggedUser.getName();
        return loggedUser;
    }
    else{
        User failedUser = User();
        failedUser.setEmail(email);
        failedUser.setId(-1);
        qDebug() << "[AUTH] A user failed the auth. Email tried: " << email;
        return failedUser;
    }
}

/*
QIcon getProfilePic(int id){

}
 
QList<QString> getDocuments(int id){

}
 */