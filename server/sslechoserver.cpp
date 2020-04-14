//
// Created by Emanuele Munafò on 04/04/2020.
//

#include "sslechoserver.h"
#include "QtWebSockets/QWebSocketServer"
#include "QtWebSockets/QWebSocket"
#include "Parser.h"
#include "PacketDef.h"
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

    // Send back (echoing) the packets?
    /*
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

    // Parsing.
    Parser parser;
    Packet packet;

    // Convert again qbytearray to vector of byte for now
    const unsigned char* begin = reinterpret_cast<unsigned char*>(rcvd_packet.data());
    const unsigned char* end = begin + rcvd_packet.length();
    std::vector<uint8_t> buf( begin, end );
    // End casting

    uint8_t *p = buf.data();
    size_t plen = buf.size();
    while (plen > 0) {
        size_t bytesRead = 0;
        if (parser.parse(p, plen, bytesRead, packet)) {
            // At this point the `packet` is complete.
            qDebug() << "[INFO] Parsed new packet: type=" << packet.type << " size=" << packet.size;
        }
        p += bytesRead;
        plen -= bytesRead;
    }
    // packet object and his field are instantiated now
    switch(packet.type){

        case PACK_TYPE_PING:
            qDebug() << "Ping received";
            break;
        case PACK_TYPE_LOGIN_REQ:
            qDebug() << "Login request";
            // Create user class
            // (implicitly or explicitly generate key= signature)
            this->tryLogin(packet);
            break;
        default:
            qDebug() << "Uknown packet type: " << packet.type;
            break;
    }

    return;
}

void SslEchoServer::tryLogin(Packet rcvd_packet) {
    /*

     user = new User(); // remember to delete on disconnect
     if(!user->verify_login){
        send_login_error()
        del user;
        return false;
     }
     user->generateSignature();

     */

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
}