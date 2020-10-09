#pragma once

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

User addUser(QString username, QString password, QString name, QString surname, QIcon profilePic);

User *checkUserLoginData(QString email, QString password);

QImage loadProfilePic(int id);

bool saveProfilePic(int id, QImage newIcon);

QVector<QString> getDocuments(int userId);

bool checkDocPermission(int docId, int userId);

bool createDoc(QString docName, int userId);

QString createInvite(int docId);

bool acceptInvite(QString invURI, int userId);

bool addDocPermission(int docId, int userId);

QString GetRandomString(int randomStringLength);

void saveToDisk(QVector<QVector<QSymbol>> qdoc, int docId);

QVector<QVector<QSymbol>> loadFromDisk(int docId);

int docIdByName(QString docName, int userId);

QVector<QString> docByInvURI(QString invURI);
