//
// Created by dimar on 25/09/2020.
//

#include "DBConfClient.h"
#include <QtCore>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "DBAuthDataClient.h"

// Perform connection to the DB
bool dbConfigure() {

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(DB_HOST);
    db.setConnectOptions(DB_OPT); // Needed for MAMP

    db.setUserName(DB_USER);
    db.setPassword(DB_PASSWORD);
    db.setDatabaseName(DB_NAME);
    //db.setPort(DB_PORT);//8889

    //qDebug() << QCoreApplication::libraryPaths();
    //qDebug() << QSqlDatabase::drivers();
    bool db_res = db.open();
    if(db_res) {
        qDebug() << "[DB] Connected to DB.";
        return true;
    }
    else {
        return false;
    }
}

QStringList getUserlistFromDocument(int documentID){
    QSqlQuery query;
    QString userlistQuery = "SELECT Username FROM user WHERE ID IN (SELECT UserID FROM permission WHERE documentID='"+QString::number(documentID)+"')";
    query.exec(userlistQuery);
    QStringList userlist = QStringList();
    while (query.next()) {
        userlist.append(query.value(0).toString());
    }
    return userlist;
}
