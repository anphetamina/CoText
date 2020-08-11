
#include "dbConf.h"
#include "User.h"
#include <QtCore>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

bool dbConfigure() {

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setConnectOptions("UNIX_SOCKET=/Applications/MAMP/tmp/mysql/mysql.sock"); // Needed for MAMP

    db.setUserName("root");
    db.setPassword("lolloasd92!");
    db.setDatabaseName("CoText");
    //db.setPort(3306);//8889

    bool ok = db.open();
    //qDebug() << ok;
    //qDebug() << QCoreApplication::libraryPaths();
    //qDebug() << QSqlDatabase::drivers();

    if(db.open()) {
        qDebug() << "[DB] Connected to DB.";
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
    qDebug() << "[INFO] Getting user list.. (username, id)";
     query.exec("SELECT username, id FROM User WHERE 1");
     while (query.next()) {
        QString username = query.value(0).toString();
        int id = query.value(1).toInt();
        qDebug() << "\t(" << username << ", " << id << ")";
     }
}

User checkLoginData(QString email, QString password){
    QSqlQuery query;
    QString hashedpassword = password;
    qDebug() << "[AUTH] Trying authentication for Mario Rossi (test@test.test)";
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
        qDebug() << "[AUTH] New user authenticated with success." << endl << "\tRetrieved info = [Email: " << loggedUser.getEmail() << "; Name:" << loggedUser.getName() << "]";
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


QIcon loadProfilePic(int id){
    QString pictureFileName = QString::number(id)+".png";
    QIcon myicon = QIcon("./profilePictures/"+pictureFileName);
}

bool saveProfilePic(int id, QIcon newIcon){
    QString pictureFileName = QString::number(id)+".png";
    QPixmap pixmap = newIcon.pixmap(64);
    return pixmap.save("./profilePictures/"+pictureFileName);
}

QList<QString> getDocuments(int userId){
    QList<QString> docList = QList<QString>();
    QSqlQuery query;
    QString quserId = QString::number(userId);
    query.exec("SELECT documentid, documentname, documentpath FROM User WHERE userid="+quserId);
     while (query.next()) {
         int id = query.value(0).toInt();
         QString docName = query.value(1).toString();
         QString docPath = query.value(2).toString();
         docList.insert(0, docName);
        //qDebug() << username << id;
     }
     return docList;
}
