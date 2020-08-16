//
// Created by Emanuele Munafò on 04/04/2020.
//

#include "sslechoclient.h"
#include "PacketDef.h"
#include "PingPacket.h"
#include "LoginPacket.h"
#include "gui/TextEditor.h"
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
    QTextCharFormat cf = QTextCharFormat();
    QSymbol qs = QSymbol(qc, test, sym_position, cf);
    Message msg = Message(MSG_INSERT_SYM, qs, 3);
    //msg.send(m_webSocket);
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
                qDebug() << "---";
                qDebug() << "[INFO] Parsed new packet. Type: " << mType;
                QWebSocket *pClient = qobject_cast<QWebSocket *>(sender());
                dispatch(packetH, pClient);
            } else {
                qDebug() << "---";
                qDebug() << "[ERROR] Unknown packet type!\nUknType: " << mType;
            }
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
            if ( loggedUser.isLogged() ){
                qDebug() << "[AUTH] Logged in as: " << loggedUser.getEmail();
            }
            else {
                qDebug() << "[AUTH] FAILED. See the server for the log.";
            }
            pServer = qobject_cast<QWebSocket *>(sender());

            break;
        }

        case (PACK_TYPE_MSG): {
            Message *msg = dynamic_cast<Message *>(rcvd_packet.get());
            switch (msg->getType()) {
                case(MSG_INSERT_SYM): {
                    emit insertReceived(msg->getS());
                    break;
                }

                case(MSG_ERASE_SYM): {
                    emit eraseReceived(msg->getS());
                    break;
                }
            }
            break;
        }

        case (PACK_TYPE_CURSOR_POS): {
            CursorPacket *cp = dynamic_cast<CursorPacket *>(rcvd_packet.get());
            emit updateCursorReceived(cp->getuserId(), cp->getnewPosition());
            break;
        }
    }
}

void SslEchoClient::sendInsert(std::vector<QSymbol> symbols, int siteId) {
    for (QSymbol symbol : symbols) {
        Message msg = Message(MSG_INSERT_SYM, symbol, siteId);
        msg.send(*pServer);
//        qDebug() << "sent" << ((symbol.getC() == '\n') ? "LF" : QString(symbol.getC())) << "(" << type << ")";
    }
}

void SslEchoClient::sendErase(std::vector<QSymbol> symbols, int siteId) {
    for (QSymbol symbol : symbols) {
        Message msg = Message(MSG_ERASE_SYM, symbol, siteId);
        msg.send(*pServer);
//        qDebug() << "sent" << ((symbol.getC() == '\n') ? "LF" : QString(symbol.getC())) << "(" << type << ")";
    }
}

void SslEchoClient::sendCursor(qint32 userId, qint32 position) {
    CursorPacket cp = CursorPacket(userId, position);
    cp.send(*pServer);
}

void SslEchoClient::connectToEditor(TextEditor* te) {
//kink to
    connect(this, &SslEchoClient::insertReceived, te, &TextEditor::remoteInsert);
    connect(this, &SslEchoClient::eraseReceived, te, &TextEditor::remoteErase);
    connect(this, &SslEchoClient::insertBlockReceived, te, &TextEditor::remoteInsertBlock);
    connect(this, &SslEchoClient::eraseBlockReceived, te, &TextEditor::remoteEraseBlock);
    connect(this, &SslEchoClient::updateCursorReceived, te, &TextEditor::updateCursor);
    connect(te, &TextEditor::symbolsInserted, this, &SslEchoClient::sendInsert);
    connect(te, &TextEditor::symbolsErased, this, &SslEchoClient::sendErase);
    connect(te, &TextEditor::cursorPositionChanged, this, &SslEchoClient::sendCursor);

}

//    // Save the secret key that will be used