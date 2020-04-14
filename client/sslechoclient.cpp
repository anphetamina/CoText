//
// Created by Emanuele Munafò on 04/04/2020.
//

#include "sslechoclient.h"
#include "Packet.h"
#include "Parser.h"
#include "PacketDef.h"
#include <QtCore/QDebug>
#include <QtWebSockets/QWebSocket>
#include <QCoreApplication>

QT_USE_NAMESPACE

//! [constructor]
SslEchoClient::SslEchoClient(const QUrl &url, QObject *parent) :
        QObject(parent)
{
    connect(&m_webSocket, &QWebSocket::connected, this, &SslEchoClient::onConnected);
    connect(&m_webSocket, QOverload<const QList<QSslError>&>::of(&QWebSocket::sslErrors),
            this, &SslEchoClient::onSslErrors);
    m_webSocket.open(QUrl(url));
}
//! [constructor]

//! [onConnected]
void SslEchoClient::onConnected()
{
    qDebug() << "WebSocket connected";
    // Set callback for text message
    connect(&m_webSocket, &QWebSocket::textMessageReceived,
            this, &SslEchoClient::onTextMessageReceived);

    // And set callback for binary msg
    connect(&m_webSocket, &QWebSocket::binaryMessageReceived,
            this, &SslEchoClient::onBinaryMessageReceived);
    //this->sendPing();
    this->sendLogin();
    //m_webSocket.sendTextMessage(QStringLiteral("Hello, world!"));
}
//! [onConnected]

//! [onTextMessageReceived]
void SslEchoClient::onTextMessageReceived(QString message)
{
    qDebug() << "Message received:" << message;
    qApp->quit();
}

//! [onTextMessageReceived]
void SslEchoClient::onBinaryMessageReceived(QByteArray message)
{
    //qDebug() << "Message received:" << message;
    this->packetParse(message);
    qApp->quit();
}

void SslEchoClient::onSslErrors(const QList<QSslError> &errors)
{
    Q_UNUSED(errors);

    // WARNING: Never ignore SSL errors in production code.
    // The proper way to handle self-signed certificates is to add a custom root
    // to the CA store.

    m_webSocket.ignoreSslErrors();
}
//! [onTextMessageReceived]

void SslEchoClient::sendPing() {
    // Create buffer
    std::vector<uint8_t> buf;

    // Define fields, serialize and send packets.
    // Ping packet.
    Packet pkt1;
    pkt1.header = 0xAF;
    pkt1.flags = 0x00;
    pkt1.type = PACK_TYPE_PING;
    pkt1.size = 3;
    pkt1.data = {0x11, 0x22, 0x33};
    pkt1.signature = 0xFF;
    pkt1.serialize(buf);
    QByteArray* qbuf = new QByteArray(reinterpret_cast<const char*>(buf.data()), buf.size());

    qDebug() << "Ping sent";
    m_webSocket.sendBinaryMessage(*qbuf);
    delete qbuf;

}

void SslEchoClient::sendLogin() {
    // Create buffer
    std::vector<uint8_t> buf;

    // Define fields, serialize and send packets.
    // Ping packet.
    Packet pkt1;
    pkt1.header = 0xAF;
    pkt1.flags = 0x00;
    pkt1.type = PACK_TYPE_LOGIN_REQ;
    pkt1.size = 3;
    pkt1.data = {0x11, 0x22, 0x33};
    pkt1.signature = 0xFF;
    pkt1.serialize(buf);
    QByteArray* qbuf = new QByteArray(reinterpret_cast<const char*>(buf.data()), buf.size());

    // Send login message
    qDebug() << "Login sent";
    m_webSocket.sendBinaryMessage(*qbuf);
    delete qbuf;

}


void SslEchoClient::packetParse(QByteArray rcvd_packet) {

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
            qDebug() << "[INFO] Parsed new packet: type=" << packet.type << " size=" << packet.size << "sig=" << packet.signature;
        }
        p += bytesRead;
        plen -= bytesRead;
    }

    // packet object and his field are instantiated now
    switch(packet.type){
        case PACK_TYPE_PING:
            qDebug() << "Pong received";
            break;
        case PACK_TYPE_LOGIN_RES:
            qDebug() << "Login response";

            break;
        default:
            qDebug() << "Uknown packet type: " << packet.type;
            break;
    }

    return;
}


//    // Save the secret key that will be used