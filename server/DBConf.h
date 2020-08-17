
#ifndef DBCONF_H
#define DBCONF_H

#endif // DBCONF_H

#include <QSqlDatabase>
#include <QtCore>
#include "User.h"
#include "QSymbol.h"

extern QString HostName;
extern QString UserName;
extern QString dbPass;
extern QString dbName;
extern int port;

bool dbConfigure();
bool getUserlist();
User checkLoginData(QString email, QString password);
User* checkUserLoginData(QString email, QString password);
QIcon loadProfilePic(int id);
QVector<QString> getDocuments(int userId);
bool checkDocPermission(int docId, int userId);
bool createDoc(QString docName, int userId);
bool addDocPermission(int docId, int userId);
QString GetRandomString(int randomStringLength);
void saveToDisk(QVector<QVector<QSymbol>> qdoc, int docId);
QVector<QVector<QSymbol>> loadFromDisk(int docId);
int docIdByName(QString docName, int userId);