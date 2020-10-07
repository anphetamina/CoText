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
#include <AccountPacket.h>

QT_USE_NAMESPACE

//! [constructor]
SslEchoServer::SslEchoServer(quint16 port, QObject *parent) :
        QObject(parent),
        m_pWebSocketServer(nullptr) {
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

    if (m_pWebSocketServer->listen(QHostAddress::Any, port)) {
        qDebug() << "SSL Echo Server listening on port" << port;
        connect(m_pWebSocketServer, &QWebSocketServer::newConnection,
                this, &SslEchoServer::onNewConnection);
        connect(m_pWebSocketServer, &QWebSocketServer::sslErrors,
                this, &SslEchoServer::onSslErrors);
    } else {
        qDebug() << "Cant listen";
    }

}
//! [constructor]

SslEchoServer::~SslEchoServer() {
    m_pWebSocketServer->close();
    //qDeleteAll(m_clients.begin(), m_clients.end());
}

//! [onNewConnection]
void SslEchoServer::onNewConnection() {
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    qDebug() << "Client connected:" << pSocket->peerName() << pSocket->origin() << pSocket->peerAddress() << "@"
             << pSocket->peerPort();

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
void SslEchoServer::processTextMessage(QString message) {
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient) {
        pClient->sendTextMessage(message);
    }
}
//! [processTextMessage]

//! [processBinaryMessage]
void SslEchoServer::processBinaryMessage(QByteArray message) {
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

/**
 * When a client disconnect from the server many actions are perfomed
 * He is removed from the clientMapping[pClient] internal structures associated to him.
 * Client is logged out
 * Its current opened document is closed
 *
 * @param closedDocId
 * @param client
 */
void SslEchoServer::socketDisconnected() {
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
        closeDocumentById(closedDocId, client);
        // Remove client element from map and close the socket
        clientMapping.remove(pClient);

        pClient->close();
        pClient->deleteLater();
    }
}

/**
 * Close a document given its a id.
 * Every time a document is closed it is also saved.
 *
 * @param closedDocId
 * @param client
 */
bool SslEchoServer::closeDocumentById(int closedDocId, QSharedPointer<Client> client) {
    // Check if there was any document open
    if (closedDocId > 0) {
        // Remove from document open map
        this->findAndDeleteFromDoclist(client);

        // Decrease the counter of current online user per document
        editorMapping[closedDocId]->connectedUsersDecrease();
        qDebug() << "[DOC_CLOSE] Remaining online user in the same document: "
                 << editorMapping[closedDocId]->getConnectedUsers();
        // Every time a user disconnect itself the server save a copy
        saveToDisk(toQVector(editorMapping[closedDocId]->getSymbols()), closedDocId);
        // if last user online is disconnecting,  delete the editorMapping entry
        if (editorMapping[closedDocId]->getConnectedUsers() == 0)
            editorMapping.remove(closedDocId);
        // Send to all the the user connected to the document that was just closed by the client the new userlist
        sendUpdatedOnlineUserByDocId(closedDocId);
    }
}

void SslEchoServer::onSslErrors(const QList<QSslError> &) {
    // Which one? Tri to handle it?
    qDebug() << "Ssl errors occurred";
}
//! [socketDisconnected]

/**
 * Get raw data from socket and wait to receive the full packet.
 * If the type is the valid range call dispatch()
 *
 * @param rcvd_packet
 */
void SslEchoServer::packetParse(QByteArray rcvd_packet) {

    // Create a new packet buffer (used to w8 and receive for the full packet)
    PacketBuffer *pBuffer = new PacketBuffer();
    //qDebug() << rcvd_packet;
    //Create a data stream (used to deserialize the rcvd bytearray  to a structured packet)
    QDataStream streamRcv(&rcvd_packet, QIODevice::ReadOnly);

    // If the packet buffer is empty parse (deserialize) the headers field (MAGIC_VAL|Flags|type|payloadLen)
    if (pBuffer->getDataSize() == 0) {
        streamRcv >> *pBuffer;
    }
    QByteArray payload = rcvd_packet.mid(4 + sizeof(quint32));//header+Payloadlen skip
    pBuffer->append(payload);

    if (pBuffer->isComplete()) {

        QDataStream dataStream(pBuffer->bufferPtr(), QIODevice::ReadWrite);
        quint8 mType = (quint8) pBuffer->getType();

        try {
            // Create an empty packet and read the fields by deserializing the data stream into a structured Packet
            PacketHandler packetH = PacketBuilder::Container(mType);
            packetH->read(dataStream);
            // Clear the buffer when a full packet is received (we are ready for the next one!)
            pBuffer->clearBuffer();

            // If the type is correct TODO: add HeadID check
            if (mType == PACK_TYPE_PING || mType <= PACK_TYPE_LAST_CODE) {
                qDebug() << "[INFO] Parsed new packet. Type: " << mType;
                QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
                dispatch(packetH, pClient);
            } else
                qDebug() << "[ERROR] Unknown packet type!\nUknType: " << mType;
        }
        catch (std::exception me)//(MyException& me)
        {
            qDebug() << me.what();
            pBuffer->clearBuffer();
            //socketAbort(m_webSocket);				// Terminate connection with the client
        }
    }
}
/**
 * Elaborate a packet and perform actions based on its type.
 * Look at PacketDef.h header file for their definition.
 *
 * @param rcvd_packet
 * @param pClient
 */
void SslEchoServer::dispatch(PacketHandler rcvd_packet, QWebSocket *pClient) {
    QSharedPointer<Client> client = clientMapping[pClient];
    //qDebug() << rcvd_packet.get();  // print packet as hex

    switch (rcvd_packet->getType()) {
        // Remeber to add {} scope to avoid jump from switch compilation error
        case (PACK_TYPE_PING): {
            PingPacket *ping = dynamic_cast<PingPacket *>(rcvd_packet.get());
            qDebug() << "[PING] Debug text: " << ping->getDebugMsg();
            break;
        }
        case (PACK_TYPE_ACC_CREATE): {
            AccountCreationPacket *accReq = dynamic_cast<AccountCreationPacket *>(rcvd_packet.get());
            User loggedUser = addUser(accReq->getUsername(), accReq->getHashedPassword(), accReq->getName(),
                                      accReq->getSurname(), accReq->getProfilePic());
            AccountOkPacket aop = AccountOkPacket(loggedUser);
            aop.send(*pClient);
            break;
        }
        case (PACK_TYPE_LOGIN_REQ): {
            LoginReqPacket *loginReq = dynamic_cast<LoginReqPacket *>(rcvd_packet.get());
            QString username = loginReq->getUsername();
            QString password = loginReq->gethashedPassword();

            User *loggedUser = checkUserLoginData(username, password);
            client->setAsLogged(loggedUser);

            // Check if user changed connections (Do after login, userID is used)
            this->pruneOldConnectionsIfAny(client, pClient);

            // Send loginOk
            LoginOkPacket lop = LoginOkPacket(*loggedUser);
            lop.send(*pClient);

            if (loggedUser->isLogged()) {
                // Send user document list
                QVector<QString> docList = getDocuments(loggedUser->getId());
                DocumentListPacket dlp = DocumentListPacket(loggedUser->getId(), docList);
                dlp.send(*pClient);
            }
            //qDebug() << loginReq->getUsername();
            break;
        }


        case (PACK_TYPE_LOGOUT_REQ): {
            LogoutReqPacket *logoutReq = dynamic_cast<LogoutReqPacket *>(rcvd_packet.get());

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

        case (PACK_TYPE_MSG): {
            Message *msg = dynamic_cast<Message *>(rcvd_packet.get());
            qDebug() << "[MSG] New symbol received." << endl << "\tChar: " << msg->getQS().getC() << " SiteId: "
                     << msg->getSiteId();
            // Broadcast to all the connected client of a document
            QList<QSharedPointer<Client>> onlineClientPerDoc = documentMapping[getDocIdOpenedByUserId(
                    client->getUserId())]; //TODO: deccoment and delete for
            for (QSharedPointer<Client> onlineClient : onlineClientPerDoc) {
                if (onlineClient != client && client->isLogged()) {
                    msg->send(*onlineClient->getSocket());
                    qDebug() << "\tfrom: " << pClient->peerPort() << "sent to "
                             << onlineClient->getSocket()->peerPort();
                }
            }
            // Run actions on the CRDT instances of the server (one for each document)
            int docId = getDocIdOpenedByUserId(client->getUserId());
            switch (msg->getType()) {
                case (MSG_INSERT_SYM): {
                    std::pair<int, int> pos = editorMapping[docId]->remoteInsert(msg->getQS());
                    break;
                }

                case (MSG_ERASE_SYM): {
                    std::pair<int, int> pos = editorMapping[docId]->remoteErase(msg->getQS());
                    break;
                }
            }

            // Full broadcast (no per document behaviour) follows. *For quick and dirty Debug only usage*
            /*for (QWebSocket* onlineClient : clientMapping.keys()) {
                if(onlineClient != pClient) {
                    msg->send(*(onlineClient));
                    qDebug() << "from: " << pClient->peerPort() << "sent to " << onlineClient->peerPort() ;
                }
            }*/

            break;
        }
        case (PACK_TYPE_BIGMSG): {
            BigMessage *bmsg = dynamic_cast<BigMessage *>(rcvd_packet.get());

            // Broadcast to all the connected client of a document
            QList<QSharedPointer<Client>> onlineClientPerDoc = documentMapping[getDocIdOpenedByUserId(
                    client->getUserId())]; //TODO: deccoment and delete for
            for (QSharedPointer<Client> onlineClient : onlineClientPerDoc) {
                if (onlineClient != client && client->isLogged()) {
                    bmsg->send(*onlineClient->getSocket());
                    qDebug() << "\tfrom: " << pClient->peerPort() << "sent to "
                             << onlineClient->getSocket()->peerPort();
                }
            }
            // Run actions on the CRDT instances of the server (one for each document)
            int docId = getDocIdOpenedByUserId(client->getUserId());
            //Call remoteInsertBlock for server
            //remoteInsertBlock(docId, bmsg->getQSS());

            switch (bmsg->getType()) {
                case (MSG_INSERT_SYM): {
                    for (auto qsymbols : bmsg->getQSS()) {
                        std::pair<int, int> pos = editorMapping[docId]->remoteInsert(qsymbols);
                    }
                    break;
                }
                case (MSG_ERASE_SYM): {
                    for (auto qsymbols : bmsg->getQSS()) {
                        std::pair<int, int> pos = editorMapping[docId]->remoteErase(qsymbols);
                    }
                    break;
                }
            }

            break;
        }
        case (PACK_TYPE_CURSOR_POS): {
            CursorPacket *cp = dynamic_cast<CursorPacket *>(rcvd_packet.get());
            //qDebug() << msg->getData();
            qDebug() << "[CP] New cursor position received." << endl << "Pos: " << cp->getnewPosition() << " User id: "
                     << cp->getuserId();
            // Broadcast to all the connected client of a document
            QList<QSharedPointer<Client>> onlineClientPerDoc = documentMapping[getDocIdOpenedByUserId(
                    client->getUserId())]; //TODO: deccoment and delete for
            for (QSharedPointer<Client> onlineClient : onlineClientPerDoc) {
                if (onlineClient != client && client->isLogged()) {
                    cp->send(*onlineClient->getSocket());
                    //qDebug() << "\tBroadcasted to from: " << pClient->peerPort() << "sent to " << onlineClient->getSocket()->peerPort() ;
                }
            }
            break;
        }
        case (PACK_TYPE_ALIGN): {
            AlignMessage *am = dynamic_cast<AlignMessage *>(rcvd_packet.get());
            //qDebug() << msg->getData();
            qDebug() << "[ALIGN] New alignment  received." << endl;
            // Broadcast to all the connected client of a document
            QList<QSharedPointer<Client>> onlineClientPerDoc = documentMapping[getDocIdOpenedByUserId(
                    client->getUserId())]; //TODO: deccoment and delete for
            for (QSharedPointer<Client> onlineClient : onlineClientPerDoc) {
                if (onlineClient != client && client->isLogged()) {
                    am->send(*onlineClient->getSocket());
                    //qDebug() << "\tBroadcasted to from: " << pClient->peerPort() << "sent to " << onlineClient->getSocket()->peerPort() ;
                }
            }
            break;
        }

        case (PACK_TYPE_DOC_CREATE): {
            DocumentCreatePacket *dcp = dynamic_cast<DocumentCreatePacket *>(rcvd_packet.get());

            if (!client->isLogged() || client->getUserId() != dcp->getuserId()) {
                break;
            }

            createDoc(dcp->getdocName(), dcp->getuserId());

            break;
        }

        case (PACK_TYPE_DOC_ASKLIST): {
            DocumentAskListPacket *dalp = dynamic_cast<DocumentAskListPacket *>(rcvd_packet.get());

            if (!client->isLogged() || client->getUserId() != dalp->getuserId()) {
                break;
            }

            DocumentListPacket dlp = DocumentListPacket(dalp->getuserId(), getDocuments(dalp->getuserId()));
            dlp.send(*pClient);
            break;
        }

        case (PACK_TYPE_DOC_OPEN): {
            if (!client->isLogged()) {
                break;
            }

            DocumentOpenPacket *dop = dynamic_cast<DocumentOpenPacket *>(rcvd_packet.get());

            // Get DocID and check permission of the user for that doc
            int docId = docIdByName(dop->getdocName(), dop->getuserId());
            qDebug() << "[SERVER] DocumentOpenPacket received docId = " << docId << " docName = " << dop->getdocName()
                     << " userId = " << dop->getuserId();

            // If the user has the privilege for opening, perform the whole loading operation
            QVector<QVector<QSymbol>> doc = remoteOpenDocument(docId, client);
            // And send the document content
            DocumentOkPacket dokp = DocumentOkPacket(docId, dop->getdocName(), doc);
            dokp.send(*pClient);
            // Send current online userlist for the given document
            sendUpdatedOnlineUserByDocId(docId);
            break;
        }
            /*
            case(PACK_TYPE_DOC_CLOSE): {
                if (!client->isLogged()) {
                    break;
                }
                DocumentClosePacket* dcp = dynamic_cast<DocumentClosePacket*>(rcvd_packet.get());
                int docId = docIdByName(dcp->getdocName(), dcp->getuserId());
                //checkDocPermission(docId, dcp->getuserId());
                if(docId < 0 ){ // doesnt have permission (no document was found with that name associated to that user)
                    DocumentOkPacket dokp = DocumentOkPacket(-1, dop->getdocName(), QVector<QVector<QSymbol>>());
                    dokp.send(*pClient);
                    break;
                }

                break;
            }*/
        case (PACK_TYPE_DOC_DEL): {
            if (!client->isLogged()) {
                break;
            }
            break;
        }

        case (PACK_TYPE_DOC_ASKSURI): {
            if (!client->isLogged()) {
                break;
            }
            DocumentAskSharableURIPacket *msg = dynamic_cast<DocumentAskSharableURIPacket *>(rcvd_packet.get());
            qDebug() << "ASK URI PACKET received userId: " << msg->getuserId() << " docId: " << msg->getdocId()
                     << " URI : " << msg->getURI();
            qint32 userId = msg->getuserId();
            QString invCode = msg->getURI();

            // Check if this is a request invite (URI is empty) or an invitation accept
            if (invCode.size() > 0) {
                // Invitation accept (in this case we receive docId=0)
                qDebug() << "ASK URI PACKET invite accepted";

                QVector<QString> doc = docByInvURI(invCode);
                int docId = doc[0].toInt();
                QString docName = doc[1];
                if (acceptInvite(invCode, userId)) {
                    // If the user has the privilege for opening, perform the whole loading operation
                    QVector<QVector<QSymbol>> doc = remoteOpenDocument(docId, client);
                    // And send the document content
                    DocumentOkPacket dokp = DocumentOkPacket(docId, docName, doc);
                    dokp.send(*pClient);
                    // Send current online userlist for the given document
                    sendUpdatedOnlineUserByDocId(docId);
                } else {
                    qDebug() << "INVITATION NOT ACCEPTED";
                }

            } else {
                int docId = msg->getdocId();
                // Check permission of the user for that doc
                if (!checkDocPermission(docId, userId)) {
                    qDebug() << "ASK URI PACKET permission denied";
                    break;
                }
                qDebug() << "ASK URI PACKET generate invitation code";
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

/**
 * Delete  one user from the document internal structure documentMapping[docId][userId].
 *
 * @param client
 */
bool SslEchoServer::findAndDeleteFromDoclist(QSharedPointer<Client> client) {
    bool wasRemoved = false;
    for (auto it = documentMapping.begin(); it != documentMapping.end();) {
        QList<QSharedPointer<Client>> onlineClientPerDoc = it.value();
        for (QSharedPointer<Client> onlineClient : onlineClientPerDoc) {
            if (onlineClient->getUserId() == client->getUserId()) {
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

/**
 * Check if the user has already a connection associated to him (ie. connected on a second device)
 *
 * @param client
 * @param pclient
 */
void SslEchoServer::pruneOldConnectionsIfAny(QSharedPointer<Client> client, QWebSocket *pClient) {

    //TODO: Check if user already exist in the map values, (i.e. connected from 1 device first and then an other)
    QWebSocket *duplicatedConnection = nullptr;
    for (auto it = this->clientMapping.begin(); it != this->clientMapping.end();) {
        if ((it.value()->getSocket() != pClient) && (it.value()->getUserId() == client->getUserId())) {
            duplicatedConnection = it.value()->getSocket();
            // Set user as logged out
            it.value()->logout();
            qDebug() << "[WARN] A user (#" << client->getUserId() << ") is now using a new connection.";
            break; // uncomment for multiple document
        } else {
            ++it;
        }
    }
    if (duplicatedConnection != nullptr) {
        //this->clientMapping.remove(duplicatedConnection);
        duplicatedConnection->close();  // Close gracefully connection to the old client instance
    }

}

/**
 * Send to each user connected to a document the new list of online user
 *
 * @param docId
 */
void SslEchoServer::sendUpdatedOnlineUserByDocId(int docId) {
    if (docId < 0)
        return;

    // Compose the updated userlist of online user per document
    QVector<User> onlineUserPerDoc = {};
    for (QSharedPointer<Client> onlineClient : documentMapping[docId]) {
        if (onlineClient->isLogged()) {
            onlineUserPerDoc.insert(0, onlineClient->getUser());
        }
    }
    // Send to all the client the new userlist
    for (QSharedPointer<Client> onlineClient : documentMapping[docId]) {
        if (onlineClient->isLogged()) {
            DocumentBeaconOnlineUsers bou = DocumentBeaconOnlineUsers(onlineUserPerDoc, docId);
            bou.send(*onlineClient->getSocket());
        }
    }
}

/**
 * Return the documentId currently opened by the user.
 * Return -1 if the user has no document opened
 *
 * @param userId
 * @return docId
 */
int SslEchoServer::getDocIdOpenedByUserId(int userId) {
    for (auto it = documentMapping.begin(); it != documentMapping.end();) {
        QList<QSharedPointer<Client>> onlineClientPerDoc = it.value();
        for (QSharedPointer<Client> onlineClient : onlineClientPerDoc) {
            if (onlineClient->getUserId() == userId) {
                return it.key();
            }
        }
        it++;
    }
    return -1;
}

/**
 * Check if there is already an instance of crdt (SharedEditor) running for that document.
 *
 * Return true if there is an instance, false otherwise.
 *
 * @param docId
 */
bool SslEchoServer::isOpenedEditorForGivenDoc(int docId) {
    for (auto editorit = editorMapping.begin(); editorit != editorMapping.end();) {
        if (editorit.key() == docId) {
            return true;
        }
    }
    return false;
}

/**
 * Open a document given its id and the client shared pointer.
 * Return a bidimensional qvector (empty on failure).
 *
 * It performs many operation under the hood:
 * close already opened doc, check permission and finally load the document.
 *
 * @param docId
 * @param client
 * @return document
 */
QVector<QVector<QSymbol>> SslEchoServer::remoteOpenDocument(int docId, QSharedPointer<Client> client) {

    int closedDocId = getDocIdOpenedByUserId(client->getUserId());
    closeDocumentById(closedDocId, client);

    //checkDocPermission(docId, client->getUserId());
    if (docId < 0) { // doesnt have permission (no document was found with that name associated to that user)
        return QVector<QVector<QSymbol>>();
    }
    // Set the document in the packet as the current opened doc
    documentMapping[docId].insert(0, client);

    //** Send the content of the document
    QVector<QVector<QSymbol>> qsymbols;
    //Se non ancora aperto da nessun utente online carico da disco e inizializzo sharedEditor (istanza CRDT)
    if (!isOpenedEditorForGivenDoc(docId)) {
        qsymbols = loadFromDisk(docId);
        QSharedPointer<SharedEditor> se(new SharedEditor(9999));
        std::vector<std::vector<QSymbol>> symbols = toVector(qsymbols);
        se->setSymbols(symbols);
        editorMapping.insert(docId, se);
    } else // Altrimenti prendo lo stato soltanto
    {
        std::vector<std::vector<QSymbol>> symbols = editorMapping[docId]->getSymbols();
        qsymbols = toQVector(symbols);
        editorMapping[docId]->connectedUsersIncrease();
    }
    return qsymbols;
}
//TODO: delete and call distructor for crdt istance after last user logged out