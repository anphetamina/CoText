//
// Created by Emanuele Munafò on 04/04/2020.
//

#include "sslechoserver.h"
#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include "PacketDef.h"
#include "PingPacket.h"
#include "../common/LoginPacket.h"
#include "Message.h"
#include "DBConf.h"
#include "../common/DocumentPacket.h"
#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
#include <random>

QT_USE_NAMESPACE

//! [constructor]
SslEchoServer::SslEchoServer(quint16 port, QObject *parent) :
        QObject(parent),
        m_pWebSocketServer(nullptr)
{
    m_pWebSocketServer = new QWebSocketServer(QStringLiteral("SSL Echo Server"),
                                              QWebSocketServer::SecureMode,
                                              this);
    QSslConfiguration sslConfiguration;
    QFile certFile(QStringLiteral("../localhost.cert"));
    QFile keyFile(QStringLiteral("../localhost.key"));
    certFile.open(QIODevice::ReadOnly);
    keyFile.open(QIODevice::ReadOnly);
    QSslCertificate certificate(&certFile, QSsl::Pem);
    QSslKey sslKey(&keyFile, QSsl::Rsa, QSsl::Pem);
    certFile.close();
    keyFile.close();
    sslConfiguration.setPeerVerifyMode(QSslSocket::VerifyNone);
    sslConfiguration.setLocalCertificate(certificate);
    sslConfiguration.setPrivateKey(sslKey);
    m_pWebSocketServer->setSslConfiguration(sslConfiguration);

    if (m_pWebSocketServer->listen(QHostAddress::Any, port))
    {
        qDebug() << "SSL Echo Server listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &SslEchoServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::sslErrors,
                this, &SslEchoServer::onSslErrors);
    } else{
        qDebug() << "Cant listen";
    }

}
//! [constructor]

SslEchoServer::~SslEchoServer()
{
    m_pWebSocketServer->close();
    //qDeleteAll(m_clients.begin(), m_clients.end());
}

//! [onNewConnection]
void SslEchoServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    qDebug() << "Client connected:" << pSocket->peerName() << pSocket->origin() << pSocket->peerAddress() << "@" << pSocket->peerPort();

    // Create a new client object
    QSharedPointer<Client> client(new Client(pSocket));

    // in every case add to client->socket map
    clientMapping.insert(pSocket, client);

    connect(pSocket, &QWebSocket::textMessageReceived, this, &SslEchoServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived,
            this, &SslEchoServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &SslEchoServer::socketDisconnected);

}
//! [onNewConnection]

//! [processTextMessage]
void SslEchoServer::processTextMessage(QString message)
{
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        pClient->sendTextMessage(message);
    }
}
//! [processTextMessage]

//! [processBinaryMessage]
void SslEchoServer::processBinaryMessage(QByteArray message)
{
    // Parse packet
    this->packetParse(message);

    /* //Send back (echoing) the packets for debug
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
        pClient->sendBinaryMessage(message);
    */
}
//! [processBinaryMessage]


//! [socketDisconnected]
void SslEchoServer::socketDisconnected()
{
    qDebug() << "Client seems to be disconnected";
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());

    if (pClient) {
        QSharedPointer<Client> client = clientMapping[pClient];

        if (client->isLogged()) {
            qDebug() << "User " << client->getEmail() << " disconnected";
        }

        client->logout();
        // Get opened document id so that i can send to all the user connected to the same document a new online user lst
        int closedDocId = getDocIdOpenedByUserId(client->getUserId());
        if(closedDocId > 0) {
            // Remove from documentopen map
            this->findAndDeleteFromDoclist(client);

            // Decrease the counter of current online user per document
            editorMapping[closedDocId]->connectedUsersDecrease();
            qDebug() << "Remaining online user in the same document: " << editorMapping[closedDocId]->getConnectedUsers();
            // Every time a user disconnect itself the server save a copy
            saveToDisk(toQVector(editorMapping[closedDocId]->getSymbols()), closedDocId);

            // if last user online is disconnecting,  delete the editorMapping entry
            if(editorMapping[closedDocId]->getConnectedUsers() == 0)
                editorMapping.remove(closedDocId);

            // Send to all the the user connected to the document that was just closed by the client the new userlist
            sendUpdatedOnlineUserByDocId(closedDocId);
        }
        // Remove client element from map and close the socket
        clientMapping.remove(pClient);

        pClient->close();
        pClient->deleteLater();
    }
}

void SslEchoServer::onSslErrors(const QList<QSslError> &)
{
    // Which one? Tri to handle it?
    qDebug() << "Ssl errors occurred";
}
//! [socketDisconnected]

void SslEchoServer::packetParse(QByteArray rcvd_packet) {

    // Create a new packet buffer (used to w8 and receive for the full packet)
    PacketBuffer* pBuffer = new PacketBuffer();
    //qDebug() << rcvd_packet;
    //Create a data stream (used to deserialize the rcvd bytearray  to a structured packet)
    QDataStream streamRcv(&rcvd_packet, QIODevice::ReadOnly);

    // If the packet buffer is empty parse (deserialize) the headers field (MAGIC_VAL|Flags|type|payloadLen)
    if (pBuffer->getDataSize() == 0) {
        streamRcv >> *pBuffer;
    }
    // Append the continuation of the packet TODO:check
    QByteArray payload = rcvd_packet.mid(4+sizeof(quint32));//header+Payoadlen skip
    pBuffer->append(payload);

    if (pBuffer->isComplete()) {

        QDataStream dataStream(pBuffer->bufferPtr(), QIODevice::ReadWrite);
        quint8 mType = (quint8)pBuffer->getType();

        try {
            // Create an empty packet and read the fields by deserializing the data stream into a structured Packet
            PacketHandler packetH = PacketBuilder::Container(mType);
            packetH->read(dataStream);
            // Clear the buffer when a full packet is received (we are ready for the next one!)
            pBuffer->clearBuffer();

            // If the type is correct TODO: add HeadID check
            if ( mType == PACK_TYPE_PING ||  mType <= PACK_TYPE_LAST_CODE )
            {
                qDebug() << "[INFO] Parsed new packet. Type: " << mType;
                QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
                dispatch(packetH, pClient);
            }
            else qDebug()  << "[ERROR] Unknown packet type!\nUknType: " << mType;
        }
        catch (std::exception me)//(MyException& me)
        {
            qDebug() << me.what();
            pBuffer->clearBuffer();
            //socketAbort(m_webSocket);				// Terminate connection with the client
        }
    }
}
/*
void SslEchoServer::tryLogin(Packet rcvd_packet) {

//     user = new User(); // remember to delete on disconnect
//     if(!user->verify_login){
//        send_login_error()
//        del user;
//        return false;
//     }
//     user->generateSignature();


    // Create user class
    // (implicitly or explicitly generate key= signature)
    std::random_device rd;
    std::uniform_int_distribution<int> dist(0,255);
    uint8_t signature = dist(rd);
    // Create mapping for this user
    //connectedCollection.append(User)
    // TODO: remember to call destructor for user on disconnect

    // send ok message
    //send_success_login()
    std::vector<uint8_t> buf;
    Packet pkt_res = rcvd_packet;
    pkt_res.type = PACK_TYPE_LOGIN_RES;
    pkt_res.signature = signature;
    pkt_res.serialize(buf);
    QByteArray* qbuf = new QByteArray(reinterpret_cast<const char*>(buf.data()), buf.size());
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    pClient->sendBinaryMessage(*qbuf);
    delete qbuf;
}*/

void SslEchoServer::dispatch(PacketHandler rcvd_packet, QWebSocket* pClient){
    QSharedPointer<Client> client = clientMapping[pClient];
    //qDebug() << rcvd_packet.get();  // print packet as hex

    switch (rcvd_packet->getType()){
        // Remeber to add {} scope to avoid jump from switch compilation error
        case(PACK_TYPE_PING): {
            PingPacket *ping = dynamic_cast<PingPacket *>(rcvd_packet.get());
            qDebug() << "[PING] Debug text: " << ping->getDebugMsg();
            break;
        }
        case(PACK_TYPE_LOGIN_REQ): {
            LoginReqPacket* loginReq = dynamic_cast<LoginReqPacket*>(rcvd_packet.get());
            QString username = loginReq->getUsername();
            QString password = loginReq->gethashedPassword();

            User* loggedUser = checkUserLoginData(username, password);
            client->setAsLogged(loggedUser);

            // Check if user changed connections (Do after login, userID is used)
            this->pruneOldConnectionsIfAny(client, pClient);

            // Send loginOk
            LoginOkPacket lop = LoginOkPacket(*loggedUser);
            lop.send(*pClient);

            if(loggedUser->isLogged()) {
                // Send user document list
                QVector<QString> docList = getDocuments(loggedUser->getId());
                DocumentListPacket dlp = DocumentListPacket(loggedUser->getId(), docList);
                dlp.send(*pClient);
            }
            //qDebug() << loginReq->getUsername();
            break;
        }


        case(PACK_TYPE_LOGOUT_REQ): {
            LogoutReqPacket* logoutReq = dynamic_cast<LogoutReqPacket*>(rcvd_packet.get());

            if (client->isLogged()) {
                qDebug() << "User " << client->getEmail() << " disconnected";
            }
            // Remove from documentopen map
            this->findAndDeleteFromDoclist(client);

            client->logout();  // --> set peer as logged out and free the memory used for User structure
            clientMapping.remove(pClient);  // Remove entry from connected peer->user mapping
            // Close websocket for that peer
            pClient->close();
            pClient->deleteLater();

            break;
        }

        case(PACK_TYPE_MSG): {
            Message* msg = dynamic_cast<Message*>(rcvd_packet.get());
            //qDebug() << msg->getData();
            qDebug() << "[MSG] New symbol received." << endl << "Char: " << msg->getQS().getC() << " SiteId: " <<  msg->getSiteId();
            // Broadcast to all the connected client of a document
            for (auto it = documentMapping.begin(); it != documentMapping.end();) { // iterate over documents and find what is openened by current user #TONOTE this works since 1 file only can be openend by a user
                //onlineClientPerDoc = documentMapping[getDocIdOpenedByUserId(client->getUserId)] TODO: deccoment and delete for
                QList<QSharedPointer<Client>> onlineClientPerDoc = it.value();
                    for (QSharedPointer<Client> onlineClient : onlineClientPerDoc) {
                        if(onlineClient != client && client->isLogged()) {
                            msg->send(*onlineClient->getSocket());
                            qDebug() << "from: " << pClient->peerPort() << "sent to " << onlineClient->getSocket()->peerPort() ;
                        }
                    }
                it++;
            }
            // Run actions on the CRDT instances of the server (one for each document)
            int docId = getDocIdOpenedByUserId(client->getUserId());
            switch (msg->getType()) {
                case(MSG_INSERT_SYM): {
                    std::pair<int, int> pos = editorMapping[docId]->remoteInsert(msg->getQS());
                    break;
                }

                case(MSG_ERASE_SYM): {
                    std::pair<int, int> pos = editorMapping[docId]->remoteErase(msg->getQS());
                    break;
                }
            }

            // Full broadcast (no per document behaviour) follows. *Debug only usage*
            /*for (QWebSocket* onlineClient : clientMapping.keys()) {
                if(onlineClient != pClient) {
                    msg->send(*(onlineClient));
                    qDebug() << "from: " << pClient->peerPort() << "sent to " << onlineClient->peerPort() ;
                }
            }*/

            break;
        }

        case(PACK_TYPE_CURSOR_POS): {
            CursorPacket *cp = dynamic_cast<CursorPacket*>(rcvd_packet.get());
            //qDebug() << msg->getData();
            qDebug() << "[CP] New cursor position received." << endl << "Pos: " << cp->getnewPosition() << " User id: " <<  cp->getuserId();
            // Broadcast to all the connected client of a document
            for (QWebSocket* onlineClient : clientMapping.keys()) {
                if(onlineClient != pClient) {
                    cp->send(*(onlineClient));
                    qDebug() << "from: " << pClient->peerPort() << "sent to " << onlineClient->peerPort() ;
                }
            }
            break;
        }
        case(PACK_TYPE_DOC_CREATE): {
            DocumentCreatePacket *dcp = dynamic_cast<DocumentCreatePacket*>(rcvd_packet.get());

            if (!client->isLogged() || client->getUserId() != dcp->getuserId()) {
                break;
            }

            createDoc(dcp->getdocName(), dcp->getuserId());
            break;
        }

        case(PACK_TYPE_DOC_ASKLIST): {
            DocumentAskListPacket *dalp = dynamic_cast<DocumentAskListPacket*>(rcvd_packet.get());

            if (!client->isLogged() || client->getUserId() != dalp->getuserId()) {
                break;
            }

            DocumentListPacket dlp = DocumentListPacket(dalp->getuserId(), getDocuments(dalp->getuserId()));
            dlp.send(*pClient);
            break;
        }

        case(PACK_TYPE_DOC_OPEN): {
            if (!client->isLogged()) {
                break;
            }

            DocumentOpenPacket* dop = dynamic_cast<DocumentOpenPacket*>(rcvd_packet.get());
            // Check if user already had an open document and delete that entry eventually #TONOTE: 1 document opened for user as of now
            this->findAndDeleteFromDoclist(client);
            // Check permission of the user for that doc
            int docId = docIdByName(dop->getdocName(), dop->getuserId());
            //checkDocPermission(docId, dop->getuserId());
            if(docId < 0 ){ // doesnt have permission (no document was found with that name associated to that user)
                DocumentOkPacket dokp = DocumentOkPacket(-1, dop->getdocName(), QVector<QVector<QSymbol>>());
                dokp.send(*pClient);
                break;
            }
            // Set the document in the packet as the current opened doc
            documentMapping[docId].insert(0, client);

            //** Send the content of the document
            QVector<QVector<QSymbol>> qsymbols;
            //Se non ancora aperto da nessun utente online carico da disco e inizializzo sharedEditor (istanza CRDT)
            if(!isOpenedEditorForGivenDoc(docId)){
                qsymbols = loadFromDisk(docId);
                QSharedPointer<SharedEditor> se(new SharedEditor(9999));
                std::vector<std::vector<QSymbol>> symbols  = toVector(qsymbols);
                se->setSymbols(symbols);
                editorMapping.insert(docId, se);
            } else // Altrimenti prendo lo stato soltanto
            {
                std::vector<std::vector<QSymbol>> symbols = editorMapping[docId]->getSymbols();
                editorMapping[docId]->connectedUsersIncrease();
            }

            DocumentOkPacket dokp = DocumentOkPacket(docId, dop->getdocName(), qsymbols);
            dokp.send(*pClient);
            // Send current online userlist for the given document
            sendUpdatedOnlineUserByDocId(docId);
            break;
        }
        case(PACK_TYPE_DOC_DEL): {
            if (!client->isLogged()) {
                break;
            }
            break;
        }

        case(PACK_TYPE_DOC_ASKSURI): {
            if (!client->isLogged()) {
                break;
            }
            DocumentAskSharableURIPacket* msg = dynamic_cast<DocumentAskSharableURIPacket*>(rcvd_packet.get());
            int docId = msg->getdocId();
            qint32 userId = msg->getuserId();
            QString invCode = msg->getURI();
            // Check permission of the user for that doc
            if(!checkDocPermission(docId, userId)){
                break;
            }
            // Check if this is a request invite (URI is empty) or an invitation accept
            if(invCode.size() > 0){
                // Invitation accept
                acceptInvite(invCode, userId);
            }
            else {
                // Generate invitation code for a given document and insert to db
                QString invCode = createInvite(docId);
                // Send the code
                DocumentAskSharableURIPacket dasup = DocumentAskSharableURIPacket(docId, userId, invCode);
                dasup.send(*pClient);
            }
            break;
        }

    }
        /* Debug send always ping for each packet received */
        /*
        for (QWebSocket* onlineClient : clientMapping.keys()) {
            PingPacket pp = PingPacket("test");
            pp.send(*onlineClient);
        }
         */
}

bool SslEchoServer::findAndDeleteFromDoclist(QSharedPointer<Client> client){
    bool wasRemoved = false;
    for (auto it = documentMapping.begin(); it != documentMapping.end();) {
        QList<QSharedPointer<Client>> onlineClientPerDoc = it.value();
        for (QSharedPointer<Client> onlineClient : onlineClientPerDoc) {
            if(onlineClient->getUserId() == client->getUserId()) {
                onlineClientPerDoc.removeOne(onlineClient);
                wasRemoved = true;
                break;
            }
        }
        *it = onlineClientPerDoc;
        it++;
    }
    return wasRemoved;
}

void SslEchoServer::pruneOldConnectionsIfAny(QSharedPointer<Client> client, QWebSocket* pClient){

    //TODO: Check if user already exist in the map values, (i.e. connected from 1 device first and then an other)
    QWebSocket* duplicatedConnection = nullptr;
    for (auto it = this->clientMapping.begin(); it != this->clientMapping.end();) {
        if ( (it.value()->getSocket() != pClient) && (it.value()->getUserId() == client->getUserId()) ) {
            duplicatedConnection = it.value()->getSocket();
            // Set user as logged out
            it.value()->logout();
            qDebug() << "[WARN] A user (#" << client->getUserId() << ") is now using a new connection." ;
            break; // uncomment for multiple document
        } else {
            ++it;
        }
    }
    if(duplicatedConnection != nullptr) {
        //this->clientMapping.remove(duplicatedConnection);
        duplicatedConnection->close();  // Close gracefully connection to the old client instance
    }

}
void SslEchoServer::sendUpdatedOnlineUserByDocId(int docId) {
    if(docId < 0)
        return;

    // Compose the updated userlist of online user per document
    QVector<User> onlineUserPerDoc = {};
    for (QSharedPointer<Client> onlineClient : documentMapping[docId]) {
        if(onlineClient->isLogged()) {
            onlineUserPerDoc.insert(0, onlineClient->getUser());
        }
    }
    // Send to all the client the new userlist
    for (QSharedPointer<Client> onlineClient : documentMapping[docId]) {
        if(onlineClient->isLogged()) {
            DocumentBeaconOnlineUsers bou = DocumentBeaconOnlineUsers(onlineUserPerDoc, docId);
            bou.send(*onlineClient->getSocket());
        }
    }
}

int SslEchoServer::getDocIdOpenedByUserId(int userId){
    for (auto it = documentMapping.begin(); it != documentMapping.end();) {
        QList<QSharedPointer<Client>> onlineClientPerDoc = it.value();
        for (QSharedPointer<Client> onlineClient : onlineClientPerDoc) {
            if(onlineClient->getUserId() == userId) {
                return it.key();
            }
        }
        it++;
    }
    return -1;
}

bool SslEchoServer::isOpenedEditorForGivenDoc(int docId){
    for (auto editorit = editorMapping.begin(); editorit != editorMapping.end();) {
        if(editorit.key() == docId){
            return true;
        }
    }
    return false;
}
//TODO: delete and call distructor for crdt istance after last user logged out