//
// Created by Emanuele Munafò on 04/04/2020.
//

#include "sslechoserver.h"
#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include "Parser.h"
#include "PacketDef.h"
#include "PingPacket.h"
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
    QFile certFile(QStringLiteral("/Users/Emanuele/CLionProjects/TogheditServer/localhost.cert"));
    QFile keyFile(QStringLiteral("/Users/Emanuele/CLionProjects/TogheditServer/localhost.key"));
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
    }
}
//! [constructor]

SslEchoServer::~SslEchoServer()
{
    m_pWebSocketServer->close();
    qDeleteAll(m_clients.begin(), m_clients.end());
}

//! [onNewConnection]
void SslEchoServer::onNewConnection()
{
    QWebSocket *pSocket = m_pWebSocketServer->nextPendingConnection();

    qDebug() << "Client connected:" << pSocket->peerName() << pSocket->origin() << pSocket->peerAddress();

    connect(pSocket, &QWebSocket::textMessageReceived, this, &SslEchoServer::processTextMessage);
    connect(pSocket, &QWebSocket::binaryMessageReceived,
            this, &SslEchoServer::processBinaryMessage);
    connect(pSocket, &QWebSocket::disconnected, this, &SslEchoServer::socketDisconnected);

    m_clients << pSocket;
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
    qDebug() << "Client disconnected";
    QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
    if (pClient)
    {
        m_clients.removeAll(pClient);
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
            if ( mType == PACK_TYPE_PING & mType <= PACK_TYPE_PING )
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

void SslEchoServer::dispatch(PacketHandler rcvd_packet, QWebSocket* m_webSocket){
    switch (rcvd_packet->getType()){
        case(PACK_TYPE_PING):
            qDebug() << rcvd_packet.get();
            PingPacket* ping = dynamic_cast<PingPacket*>(rcvd_packet.get());
            //PacketHandler response = emit loginRequest(m_webSocket, ping->getDebugMsg());
            //response->send(socket);
            qDebug() << ping->getDebugMsg();
            break;
    }
}