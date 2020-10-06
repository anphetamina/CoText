#include "DBConf.h"
#include "User.h"
#include <QtCore>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
//#include <filesystem>

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

User addUser(QString username,QString password, QString name, QString surname, QIcon profilePic){
    QSqlQuery query, query2, query3;
    QString email = username;
    QString hashedpassword = password;//perform hashing for sec. reason in production

    bool success = true;

    if(!query.exec("INSERT INTO User(username,email, name, surname, password) VALUES ('"+username+"','"+email+"','"+name+"',"+surname+"',"+hashedpassword+")")){
        success =  false;
    }

    // Check if it was  added (it should if here) and get ID
    query2.exec("SELECT username, id, email, name, surname FROM User WHERE email='"+email+"' AND password='" + hashedpassword + "'");

    if (query2.next() && success) {
        QString username = query.value(0).toString();
        int id = query.value(1).toInt();
        QString email = query.value(2).toString();
        QString name = query.value(3).toString();
        QString surname = query.value(4).toString();

        if(!saveProfilePic(id, profilePic)){
            success = false;
        }

        User loggedUser = User(id, email, name, surname);
        qDebug() << "[AUTH] New user registered with success." << endl << "\tRetrieved info = [Email: " << loggedUser.getEmail() << "; Name:" << loggedUser.getName() << "]";
        return loggedUser;
    }
    User failedUser = User();
    failedUser.setEmail(email);
    failedUser.setId(-1);
    qDebug() << "[AUTH] A user failed the auth. Email tried: " << email;
    return failedUser;

}

User* checkUserLoginData(QString email, QString password){
    /* Check login data and return a user istance. Check with the isLogged method the result of the performed auth*/
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

/*
 * Load a profile picture from disk as QICon given the userId
 */
QIcon loadProfilePic(int id){
    QString pictureFileName = QString::number(id)+".png";
    QIcon myicon = QIcon();
    QString filePath ="./profilePictures/"+pictureFileName;
    QFile file(filePath);
    QVector<QVector<QSymbol>> qdoc;
    if(!file.open(QIODevice::ReadOnly)){
        myicon = QIcon(filePath);
    }

    return myicon;
}

/*
 * Load a profile picture to disk as QICon given the userId and the icon
 */
bool saveProfilePic(int id, QIcon newIcon){
    QString pictureFileName = QString::number(id)+".png";
    QPixmap pixmap = newIcon.pixmap(64);
    return pixmap.save("./profilePictures/"+pictureFileName);
}

QVector<QString> getDocuments(int userId){
    QVector<QString> docList = QVector<QString>();
    QSqlQuery query;
    QString quserId = QString::number(userId);
    query.exec("SELECT documentid, documentname, documentpath FROM Permission WHERE userid="+quserId);
     while (query.next()) {
         int id = query.value(0).toInt();
         QString docName = query.value(1).toString();
         QString docPath = query.value(2).toString();
         docList.insert(0, docName);
     }
     return docList;
}

bool checkDocPermission(int docId, int userId){
    /* Check if a given user has the right to open/edit a given document */
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

bool createDoc(QString docName, int userId){
    QSqlQuery query, query2, query3;
    QString quserId = QString::number(userId);
    QString docPath = QString();
    query.exec("INSERT INTO Permission(documentname, documentpath, userid) VALUES ('"+docName+"','"+docPath+"',"+quserId+")");
    // Check if it was already added (userid, docID) should be UNIQUE
    query2.exec("SELECT id, documentname FROM Permission WHERE userid="+quserId+" ORDER BY ID DESC");

    if (query2.next()) {
        int id = query2.value(0).toInt();
        QString qdocId = QString::number(id);
        docName = query2.value(1).toString();
        query3.exec("UPDATE Permission SET documentid="+qdocId+" WHERE id="+qdocId);
        return true;
    } else{
        return false;
    }
}

QString createInvite(int docId){
    QSqlQuery query, query2;

    QString qdocId = QString::number(docId);
    QString docName, docPath;
    QString invURI;
    int id;
    query.exec("SELECT documentid, documentname, documentpath FROM Permission WHERE documentid="+qdocId);
    if (query.next()) {
        id = query.value(0).toInt();
        docName = query.value(1).toString();
        docPath = query.value(2).toString();
    }
    else{
        return invURI;
    }

    bool unique = false;
    while(!unique){//TODO: A better logic could be implemented for sure here, but collision rate is *really* low (and every time the invite is accepted the entry is deleted)
        QSqlQuery uquery;
        invURI = GetRandomString(20);
        uquery.exec("SELECT documentid FROM Permission WHERE URI="+invURI);
        if (!uquery.next()) {
            unique = true;
        }
    }
    query2.exec("INSERT INTO Permission(documentname,documentID, documentpath, URI) VALUES ('"+docName+"',"+qdocId+",'"+docPath+"','"+invURI+"')");
    // Check if it was already added (userid, docID) should be UNIQUE
    return invURI;
}

bool acceptInvite(QString invURI, int userId){
    /* Take the invitation uri and the userId that wants to validate the invite. Add the permission to the user*/
    QSqlQuery query, query2;
    QString quserId = QString::number(userId);
    int docId;

    // Execute the first query to check if the invURI is valid and to parse document info
    query.exec("SELECT documentid, documentname, documentpath FROM Permission WHERE URI='"+invURI+"'");
    if (!query.next()) {
        return false;
    }else{
        docId = query.value(0).toInt();
        //docName = query.value(1).toString();
    }

    // If the user already has right to that document, just delete the entry
    if(checkDocPermission(docId, userId)){
        query2.exec("DELETE Permission WHERE URI='"+invURI+"'");
        return true;
    }
    // Else update with the userId of that user who redeemed the invCode
    query2.exec("UPDATE Permission SET userid="+quserId+", URI=NULL WHERE URI='"+invURI+"'");
    return true;
}

/*Add the document permission for a given set user, document */
bool addDocPermission(int docId, int userId){
    QSqlQuery query, query2, query3;
    QString quserId = QString::number(userId);
    QString qdocId = QString::number(docId);
    QString docName, docPath;
    query.exec("SELECT documentid, documentname, documentpath FROM Permission WHERE documentid="+qdocId);
    if (query.next()) {
        int id = query.value(0).toInt();
        docName = query.value(1).toString();
        docPath = query.value(2).toString();
    }
    else{
        return false;
    }
    // Check if it was already added (userid, docID) should be UNIQUE
    query2.exec("SELECT documentid, documentname, documentpath FROM Permission WHERE userid="+quserId+" AND documentid="+qdocId);
    if (query2.next()) {
        return true;
    }
    query3.exec("INSERT INTO Permission(documentid, documentname, documentpath, userid) VALUES ("+qdocId+", '"+docName+"','"+docPath+"',"+quserId+")");
    return true;
}

void saveToDisk(QVector<QVector<QSymbol>> qdoc, int docId){
    QString qdocId = QString::number(docId);
    QFile file("doc"+qdocId+".dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);   // we will serialize the data into the file
    out << qdoc;
    file.close();
}

QVector<QVector<QSymbol>> loadFromDisk(int docId){
    QString qdocId = QString::number(docId);
    QFile file("doc"+qdocId+".dat");
    QVector<QVector<QSymbol>> qdoc;
    if(!file.open(QIODevice::ReadOnly)){
        qdoc = {{}};  // if no file is found, maybe it's an empty document for now.
    } else {
        QDataStream out(&file);   // we will serialize the data into the file
        out >> qdoc;
    }
    return qdoc;
}


QString GetRandomString(int randomStringLength=100)
{
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

    QString randomString;
    for(int i=0; i<randomStringLength; ++i)
    {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}

int docIdByName(QString docName, int userId){
    QSqlQuery query;
    QString quserId = QString::number(userId);
    query.exec("SELECT documentid FROM Permission WHERE userid="+quserId+" AND documentname='"+docName+"'");
    if (query.next()) {
        int id = query.value(0).toInt();
        return id;
    }
    else
        return -1;
}

