//
// Created by Emanuele Munafò on 04/04/2020.
//

#include "sslechoclient.h"
#include "Packet.h"
#include "PacketDef.h"
#include "PingPacket.h"
#include "LoginPacket.h"
#include "Message.h"
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
    this->sendPing();
    this->sendTest();
    //this->sendLogin();
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
    PingPacket pp = PingPacket("test");

    pp.send(m_webSocket);
    qDebug() << "Ping sent";

}

void SslEchoClient::sendTest() {
    qDebug() << "[NETWORK] ** Network Test Start ** ";
    this->authenticate("test@test.test", "test");
    qDebug() << "[NETWORK] ** Sending Message packet ** ";
    QVector<Identifier> sym_position;
    QString test("test_qstring");
    QChar qc = test.at(0);//t
    QSymbol qs = QSymbol(qc, test, sym_position);
    Message msg = Message(1, qs, 3);
    msg.send(m_webSocket);
    qDebug() << "[NETWORK] ** Network Test Packet were all sent ** ";

}

void SslEchoClient::authenticate(QString username, QString password) {
    qDebug() << "[NETWORK] ** Sending login req packet ** ";
    QString hashedPassword = password;
    LoginReqPacket lrp = LoginReqPacket(username, hashedPassword);
    lrp.send(m_webSocket);
}

void SslEchoClient::packetParse(QByteArray rcvd_packet) {
    PacketBuffer* pBuffer = new PacketBuffer();

    // Parsing.
    /*QDataStream streamRcv(&rcvd_packet, QIODevice::ReadOnly);
    //SocketBuffer& socketBuffer = clients.value(socket)->getSocketBuffer();// TODO: extend for many

    if (pBuffer->getDataSize() == 0) {
        rcvd_packet >> pBuffer;
    }*/
    pBuffer->append(rcvd_packet);

    if (pBuffer->isComplete()) {

        QDataStream dataStream(pBuffer->bufferPtr(), QIODevice::ReadWrite);
        quint8 mType = (quint8)pBuffer->getType();

        try {
            PacketHandler packetH = PacketHandler();
            packetH->read(dataStream);
            pBuffer->clearBuffer();

            if (mType == PACK_TYPE_PING)
            {
                qDebug() << "[INFO] Parsed new packet:";
                //packetHandler.process(message, socket);
            }
            else qDebug()  << "[ERROR] Uknown type: " << mType;
        }
        catch (std::exception me)//(MyException& me)
        {
            qDebug() << me.what();
            pBuffer->clearBuffer();
            //socketAbort(m_webSocket);				// Terminate connection with the client
        }
    }

    /*
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
     */
}


//    // Save the secret key that will be used