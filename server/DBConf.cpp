#include "DBConf.h"
#include "User.h"
#include <QtCore>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "DBAuthData.h"

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

bool getUserlist(){
     QSqlQuery query;
    qDebug() << "[INFO] Getting user list.. (username, id)";
     query.exec("SELECT username, id FROM User WHERE 1");
     while (query.next()) {
        QString username = query.value(0).toString();
        int id = query.value(1).toInt();
        qDebug() << "\t(" << username << ", " << id << ")";
     }
    // return qvector or some other data struct
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

User* checkUserLoginData(QString email, QString password){
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
        User* loggedUser = new User(id, email, name, surname);
        qDebug() << "[AUTH] New user authenticated with success." << endl << "\tRetrieved info = [Email: " << loggedUser->getEmail() << "; Name:" << loggedUser->getName() << "]";
        return loggedUser;
    }
    else{
        User* failedUser = new User();
        failedUser->setEmail(email);
        failedUser->setId(-1);
        qDebug() << "[AUTH] A user failed the auth. Email tried: " << email;
        return failedUser;
    }
}


QIcon loadProfilePic(int id){
    QString pictureFileName = QString::number(id)+".png";
    QIcon myicon = QIcon("./profilePictures/"+pictureFileName);
    return myicon;
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
    query.exec("SELECT documentid, documentname, documentpath FROM Permission WHERE userid="+quserId);
     while (query.next()) {
         int id = query.value(0).toInt();
         QString docName = query.value(1).toString();
         QString docPath = query.value(2).toString();
         docList.insert(0, docName);
        //qDebug() << username << id;
     }
     return docList;
}

bool checkDocPermission(int docId, int userId){
    QSqlQuery query;
    QString quserId = QString::number(userId);
    QString qdocId = QString::number(docId);

    query.exec("SELECT documentid, documentname, documentpath FROM Permission WHERE userid="+quserId+" AND documentid="+qdocId);
    if (query.next()) {
        int id = query.value(0).toInt();
        QString docName = query.value(1).toString();
        QString docPath = query.value(2).toString();
        //qDebug() << username << id;
        return true;
    }
    else{
        return false;
    }
}

/*
void saveToDisk(QString doc){
    QFile file("docX.dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);   // we will serialize the data into the file
    out << doc;
    file.close();

}

QString loadFromDisk(){
    QFile file("docX.dat");
    QString doc;
    file.open(QIODevice::ReadOnly);
    out >> doc;
}
 */