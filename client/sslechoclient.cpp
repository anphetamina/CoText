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
    //qApp->quit();
}

//! [onTextMessageReceived]
void SslEchoClient::onBinaryMessageReceived(QByteArray message)
{
    //qDebug() << "Message received:" << message;
    this->packetParse(message);
    //qApp->quit();
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

void SslEchoClient::dispatch(PacketHandler rcvd_packet, QWebSocket* pClient) {
    //qDebug() << rcvd_packet.get();  // print packet as hex
    qDebug() << "New packet type= " << rcvd_packet->getType();
    switch (rcvd_packet->getType()) {
        // Remeber to add {} scope to avoid jump from switch compilation error
        case (PACK_TYPE_PING): {
            PingPacket *ping = dynamic_cast<PingPacket *>(rcvd_packet.get());
            qDebug() << "[PING] Debug text: " << ping->getDebugMsg();
            break;
        }

        case(PACK_TYPE_LOGIN_OK): {
            LoginOkPacket* loginOk = dynamic_cast<LoginOkPacket*>(rcvd_packet.get());
            User loggedUser = loginOk->getUser();
            qDebug() << "[AUTH] Logged in as: "<< loggedUser.getEmail();
            break;
        }
    }
}



//    // Save the secret key that will be used