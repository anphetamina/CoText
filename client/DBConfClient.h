//
// Created by dimar on 25/09/2020.
//

#ifndef COTEXT_CLIENT_DBCONFCLIENT_H
#define COTEXT_CLIENT_DBCONFCLIENT_H


#include <QSqlDatabase>
#include <QtCore>

extern QString HostName;
extern QString UserName;
extern QString dbPass;
extern QString dbName;
extern int port;

bool dbConfigure();
QStringList getUserlistFromDocument(int documentID);

#endif //COTEXT_CLIENT_DBCONFCLIENT_H
