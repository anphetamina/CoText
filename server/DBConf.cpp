#include "DBConf.h"
#include "User.h"
#include <QtCore>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "DBAuthData.h"

/**
 * Perform connection to the DB
 */
bool dbConfigure() {

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(DB_HOST);
    //db.setConnectOptions(DB_OPT); // Needed for MAMP

    db.setUserName(DB_USER);
    db.setPassword(DB_PASSWORD);
    db.setDatabaseName(DB_NAME);
    db.setPort(DB_PORT);

    //qDebug() << QCoreApplication::libraryPaths();
    qDebug() << QSqlDatabase::drivers();
    bool db_res = db.open();
    if (db_res) {
        qDebug() << "[DB] Connected to DB.";
        return true;
    } else {
        return false;
    }
}

/**
 * Print the current user in the DB
 */
bool getUserlist() {
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

/**
 * Add a user to the DB and save the profile picture
 */
User addUser(QString username, QString password, QString name, QString surname, QImage profilePic) {
    QSqlQuery query, query2, query3;
    QString email = username;
    QString hashedpassword = password;//perform hashing for sec. reason in production

    bool success = true;

    if (!query.exec(
            "INSERT INTO User(username,email, name, surname, password) VALUES ('" + username + "','" + email + "','" +
            name + "','" + surname + "','" + hashedpassword + "')")) {
        success = false;
    }

    // Check if it was  added (it should if here) and get ID
    query2.exec("SELECT username, id, email, name, surname FROM User WHERE email='" + email + "' AND password='" +
                hashedpassword + "'");

    if (query2.next() && success) {
        QString username = query2.value(0).toString();
        int id = query2.value(1).toInt();
        QString email = query2.value(2).toString();
        QString name = query2.value(3).toString();
        QString surname = query2.value(4).toString();

       /* if (!saveProfilePic(id, profilePic)) {
            success = false;
        }*/

        User loggedUser = User(id, email, name, surname);
        loggedUser.setSurname(surname);
        loggedUser.setPassword(hashedpassword);
        //User loggedUser = checkUserLoginData(email, password);
        qDebug() << "[AUTH] New user registered with success." << endl << "\tRetrieved info = [Email: "
                 << loggedUser.getEmail() << "; Name:" << loggedUser.getName() << "]";
        saveProfilePic(id, profilePic);
        return loggedUser;
    }
    User failedUser = User();
    failedUser.setEmail(email);
    failedUser.setId(-1);
    qDebug() << "[AUTH] A user failed the auth. Email tried: " << email;
    return failedUser;

}

/**
 * Update a user in the DB and save the profile picture
 */
User updateUser(int userId, QString username, QString password, QString name, QString surname, QImage profilePic) {
    QSqlQuery query, query2, query3;
    QString email = username;
    QString hashedpassword = password;//perform hashing for sec. reason in production

    bool success = true;

    if (!query.exec(
            "UPDATE User SET username = '"+username+"',email = '"+email+"', name='"+name+"', surname'"+surname+"', password='"+password+"' WHERE id="+userId+";")) {
        success = false;
    }



    if ( success) {
        User loggedUser = User(userId, email, name, surname);
        qDebug() << "[ACC] USer data updated with success." << endl << "\tRetrieved info = [Email: "
                 << loggedUser.getEmail() << "; Name:" << loggedUser.getName() << "]";
        saveProfilePic(userId, profilePic);
        return loggedUser;
    }
    User failedUser = User();
    failedUser.setEmail(email);
    failedUser.setId(-1);
    qDebug() << "[AUTH] A user failed the auth. Email tried: " << email;
    return failedUser;

}

/**
 * Check login data and return a user instance.
 * You can check with the isLogged method the result of the performed auth
 * */
User *checkUserLoginData(QString email, QString password) {
    QSqlQuery query;
    QString hashedpassword = password;
    qDebug() << "[AUTH] Trying authentication for Mario Rossi (test@test.test)";

    // Avoiding security concerns about sql for now. Security :(
    QString squery = "SELECT username, id, email, name, surname FROM User WHERE email='" + email + "' AND password='" +
                     hashedpassword + "'";
    query.exec(squery);

    if (query.next()) {
        QString username = query.value(0).toString();
        int id = query.value(1).toInt();
        QString email = query.value(2).toString();
        QString name = query.value(3).toString();
        QString surname = query.value(4).toString();
        User *loggedUser = new User(id, email, name, surname);
        loggedUser->setSurname(surname);
        loggedUser->setPassword(hashedpassword);
        qDebug() << "[AUTH] New user authenticated with success." << endl << "\tRetrieved info = [Email: "
                 << loggedUser->getEmail() << "; Name:" << loggedUser->getName() << "]";
        return loggedUser;
    } else {
        User *failedUser = new User();
        failedUser->setEmail(email);
        failedUser->setId(-1);
        qDebug() << "[AUTH] A user failed the auth. Email tried: " << email;
        return failedUser;
    }
}

/**
 * Load a profile picture from disk as QImage given the userId
 */
QImage loadProfilePic(int id) {
    QString pictureFileName = QString::number(id) + ".png";
    QImage myicon = QImage();
    QString filePath = "./profilePictures/" + pictureFileName;
    QFile file(filePath);
    QVector<QVector<QSymbol>> qdoc;
    if (!file.open(QIODevice::ReadOnly)) {
        myicon = QImage(filePath);
    }

    return myicon;
}

/**
 * Save a profile picture to disk as QImage given the userId and the icon
 */
bool saveProfilePic(int id, QImage newIcon) {
    QString pictureFileName = QString::number(id) + ".png";
    return newIcon.save("./profilePictures/" + pictureFileName);
}

QVector<QString> getDocuments(int userId) {
    QVector<QString> docList = QVector<QString>();
    QSqlQuery query;
    QString quserId = QString::number(userId);
    query.exec("SELECT documentid, documentname, documentpath FROM Permission WHERE userid=" + quserId);
    while (query.next()) {
        int id = query.value(0).toInt();
        QString docName = query.value(1).toString();
        QString docPath = query.value(2).toString();
        docList.insert(0, docName);
    }
    return docList;
}

/**
 * Check if a given user has the right to open/edit a given document
 */
bool checkDocPermission(int docId, int userId) {
    QSqlQuery query;
    QString quserId = QString::number(userId);
    QString qdocId = QString::number(docId);

    query.exec("SELECT documentid, documentname, documentpath FROM Permission WHERE userid=" + quserId +
               " AND documentid=" + qdocId);
    if (query.next()) {
        int id = query.value(0).toInt();
        QString docName = query.value(1).toString();
        QString docPath = query.value(2).toString();
        //qDebug() << username << id;
        return true;
    } else {
        return false;
    }
}

/**
 * Create a document given a name and the userId of the creator
 */
bool createDoc(QString docName, int userId) {
    QSqlQuery query0, query, query2, query3;
    QString quserId = QString::number(userId);
    QString docPath = QString();
    QString available_docName = docName;
    bool available_name = false;

    while (!available_name) {//TODO: improve logic, but good PoC
        // Check if it was already added (userid, docID) should be UNIQUE
        query0.exec(
                "SELECT id FROM Permission WHERE userid=" + quserId + "AND documentname=" + available_docName + ";");

        if (!query0.next()) {
            break;
        } else {
            available_docName.append("_copy");
        }
    }
    //docPath = available_docName+".dat";

    query.exec(
            "INSERT INTO Permission(documentname, documentpath, userid) VALUES ('" + available_docName + "','" +
            docPath + "'," +
            quserId + ")");
    // Check if it was added ; (userid, docID) should be declared as UNIQUE
    // and get docId (dirty way)
    query2.exec("SELECT id, documentname FROM Permission WHERE userid=" + quserId + " ORDER BY ID DESC");
    if (query2.next()) {
        int id = query2.value(0).toInt();
        QString qdocId = QString::number(id);
        docName = query2.value(1).toString();
        query3.exec("UPDATE Permission SET documentid=" + qdocId + " WHERE id=" + qdocId);
        return true;
    } else {
        return false;
    }
}

/**
 * Generate and return a random inv code of 20 alphanumerical chars.
 * The code is related to one document and can be redeemed by whatever user will use it as first.
 */
QString createInvite(int docId) {
    QSqlQuery query, query2;

    QString qdocId = QString::number(docId);
    QString docName, docPath;
    QString invURI;
    int id;
    query.exec("SELECT documentid, documentname, documentpath FROM Permission WHERE documentid=" + qdocId);
    if (query.next()) {
        id = query.value(0).toInt();
        docName = query.value(1).toString();
        docPath = query.value(2).toString();
    } else {
        return invURI;
    }

    bool unique = false;
    while (!unique) {//TODO: A better logic could be implemented for sure here, but collision rate is *really* low (and every time the invite is accepted the entry is deleted)
        QSqlQuery uquery;
        invURI = GetRandomString(20);
        uquery.exec("SELECT documentid FROM Permission WHERE URI=" + invURI);
        if (!uquery.next()) {
            unique = true;
        }
    }
    query2.exec(
            "INSERT INTO Permission(documentname,documentID, documentpath, URI) VALUES ('" + docName + "'," + qdocId +
            ",'" + docPath + "','" + invURI + "')");
    // Check if it was already added (userid, docID) should be UNIQUE
    return invURI;
}

/**
 * Take the invitation uri and the userId that wants to validate the invite.
 * Add the permission to the user so he can access the document linked to that code.
 * */
bool acceptInvite(QString invURI, int userId) {
    QSqlQuery query, query2;
    QString quserId = QString::number(userId);
    int docId;

    // Execute the first query to check if the invURI is valid and to parse document info
    query.exec("SELECT documentid, documentname, documentpath FROM Permission WHERE URI='" + invURI + "'");
    if (!query.next()) {
        return false;
    } else {
        docId = query.value(0).toInt();
        //docName = query.value(1).toString();
    }

    // If the user already has right to that document, just delete the entry
    if (checkDocPermission(docId, userId)) {
        query2.exec("DELETE Permission WHERE URI='" + invURI + "'");
        return true;
    }
    // Else update with the userId of that user who redeemed the invCode
    query2.exec("UPDATE Permission SET userid=" + quserId + ", URI=NULL WHERE URI='" + invURI + "'");
    return true;
}

QVector<QString> docByInvURI(QString invURI) {
    QVector<QString> doc({"", ""});
    QSqlQuery query;
    query.exec("SELECT documentid, documentname, documentpath FROM Permission WHERE URI='" + invURI + "'");
    if (query.next()) {
        doc[0] = query.value(0).toString();
        doc[1] = query.value(1).toString();
    }
    return doc;
}

/**
 * Add the document permission for a given documentId and userId.
 * Return false in case of failure.
 * Note: returns true also if the document already exist.
 */

bool addDocPermission(int docId, int userId) {
    QSqlQuery query, query2, query3;
    QString quserId = QString::number(userId);
    QString qdocId = QString::number(docId);
    QString docName, docPath;
    // The document should exist
    query.exec("SELECT documentid, documentname, documentpath FROM Permission WHERE documentid=" + qdocId);
    if (query.next()) {
        int id = query.value(0).toInt();
        docName = query.value(1).toString();
        docPath = query.value(2).toString();
    } else {
        return false;
    }
    // Check if it was already added (userid, docID) should be UNIQUE
    query2.exec("SELECT documentid, documentname, documentpath FROM Permission WHERE userid=" + quserId +
                " AND documentid=" + qdocId);
    if (query2.next()) {
        return true;
    }
    query3.exec("INSERT INTO Permission(documentid, documentname, documentpath, userid) VALUES (" + qdocId + ", '" +
                docName + "','" + docPath + "'," + quserId + ")");
    return true;
}

/**
 * Save to disk the internal representation of the document for a given documentId and bidimensional QVec
 */
void saveToDisk(QVector<QVector<QSymbol>> qdoc, int docId) {
    QString qdocId = QString::number(docId);
    QFile file("doc" + qdocId + ".dat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);   // we will serialize the data into the file
    out << qdoc;
    file.close();
}

/**
 * Save to disk the internal representation of the document alignment for a given document
 */
void saveAlignmentToDisk(QVector<AlignMessage> qadoc, int docId) {
    QString qdocId = QString::number(docId);
    QFile file("doc" + qdocId + ".adat");
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);   // we will serialize the data into the file
    out << qadoc;
    file.close();
}

/**
 * Load the internal structure that represents the alignment of a given the documentId
 */
QVector<AlignMessage> loadAlignmentFromDisk(int docId) {
    QString qdocId = QString::number(docId);
    QFile file("doc" + qdocId + ".adat");
    QVector<AlignMessage> qadoc;
    if (!file.open(QIODevice::ReadOnly)) {
        qadoc = QVector<AlignMessage> ();  // if no file is found, maybe it's an empty document for now.
    } else {
        QDataStream out(&file);   // we will serialize the data into the file
        out >> qadoc;
    }
    return qadoc;
}
/**
 * Load the internal structure that represents a document given the documentId
 */
QVector<QVector<QSymbol>> loadFromDisk(int docId) {
    QString qdocId = QString::number(docId);
    QFile file("doc" + qdocId + ".dat");
    QVector<QVector<QSymbol>> qdoc;
    if (!file.open(QIODevice::ReadOnly)) {
        qdoc = {{}};  // if no file is found, maybe it's an empty document for now.
    } else {
        QDataStream out(&file);   // we will serialize the data into the file
        out >> qdoc;
    }
    return qdoc;
}



/**
 * Generate an alphanumerical string for the given length.
 */
QString GetRandomString(int randomStringLength = 100) {
    const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");

    QString randomString;
    for (int i = 0; i < randomStringLength; ++i) {
        int index = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }
    return randomString;
}

/*
 * Returns the document Id given a documentName if the given userId has the right to access.
 * Return -1 if the document doesnt exist or the current user has not the privilege.
 */
int docIdByName(QString docName, int userId) {
    QSqlQuery query;
    QString quserId = QString::number(userId);
    query.exec("SELECT documentid FROM Permission WHERE userid=" + quserId + " AND documentname='" + docName + "'");
    if (query.next()) {
        int id = query.value(0).toInt();
        return id;
    } else
        return -1;
}

bool deleteDocument(QString docName, int userId) {
    QSqlQuery query;
    QString quserId = QString::number(userId);
    query.exec("DELETE FROM Permission WHERE userid=" + quserId + " AND documentname='" + docName + "'");
    qDebug() << "[DB CONF] deleteDocument docName = "<<docName << " userId = "<< userId;
    return true;
}

/*
 * Returns the list of all users that have permission on that document.
 */
QVector<User> getUserListByDocId(int docId) {
    QSqlQuery query;
    QString qdocId = QString::number(docId);
    QVector<User> userList = QVector<User>();
    query.exec("SELECT U.ID, U.email, U.name, U.surname FROM Permission P, User U WHERE P.UserID = U.ID AND documentid='" + qdocId + "'");
    while (query.next()) {
        int id = query.value(0).toInt();
        QString email = query.value(1).toString();
        QString name = query.value(2).toString();
        QString surname = query.value(3).toString();
        userList.append(User(id, email, name, surname));
    }
    return userList;
}
