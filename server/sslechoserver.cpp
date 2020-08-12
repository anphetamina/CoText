//
// Created by Emanuele Munafò on 04/04/2020.
//

#include "sslechoserver.h"
#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include "PacketDef.h"
#include "PingPacket.h"
#include "LoginPacket.h"
#include "Message.h"
#include "DBConf.h"
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

    qDebug() << "Client connected:" << pSocket->peerName() << pSocket->origin() << pSocket->peerAddress();

    // Create a new client object
    QSharedPointer<Client> client(new Client(pSocket));
    clientMapping.insert(pSocket, client);

    connect(pSocket, &QWebSocket::textMessageReceived, this, &SslEchoServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived,
            this, &SslEchoServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &SslEchoServer::socketDisconnected);
    //TODO: Check if user already exist in the map values, (i.e. connected from 1 device first and then an other)

    //m_clients << pSocket;
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
    {
        pClient->sendBinaryMessage(message);
    }
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

            LoginOkPacket lop = LoginOkPacket(*loggedUser);
            lop.send(*pClient);
            //qDebug() << loginReq->getUsername();
            break;
        }


        case(PACK_TYPE_LOGOUT_REQ): {
            LogoutReqPacket* logoutReq = dynamic_cast<LogoutReqPacket*>(rcvd_packet.get());

            if (client->isLogged()) {
                qDebug() << "User " << client->getEmail() << " disconnected";
            }
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
            qDebug() << "[MSG] New symbol received." << endl << "Char: " << msg->getS().getC() << " SiteId: " <<  msg->getSiteId();
            // Broadcast to all the connected client of a document

            break;
        }
        /*
        case(PACK_TYPE_DOC_OPEN): {
            // Check if user already had an open document
            // Check permission of the user for that doc
            // Set the document in the packet as the current opened doc
            //documentMapping.insert(docId, client)
            // Send the content of the document
            break;
        }
        */
    }
}