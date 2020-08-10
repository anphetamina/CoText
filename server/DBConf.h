
#ifndef DBCONF_H
#define DBCONF_H

#endif // DBCONF_H

#include <QSqlDatabase>
#include <QtCore>

extern QString HostName;
extern QString UserName;
extern QString dbPass;
extern QString dbName;
extern int port;

bool dbConfigure();
bool getUserlist();

