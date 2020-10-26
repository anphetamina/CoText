//
// Created by Emanuele Munafò on 13/04/2020.
//
#ifdef _WIN32
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501  /* Windows XP. */
#endif
#include <winsock2.h>
#endif


#include "Packet.h"
#include "PacketDef.h"
#include "PingPacket.h"
#include "LoginPacket.h"
#include "AccountPacket.h"
#include "Message.h"
#include "CursorPacket.h"
#include "DocumentPacket.h"

/** Packet **/
Packet::Packet(uint16_t type) : header(0xAF), type(type), flags(0x00) {}

Packet::~Packet() {}

// Serialize the content of the packet
// Could use QDataStream
void Packet::serialize(QByteArray buf, QDataStream &stream) {
    // Header|FLAGS|type|size|payload

    stream << header << flags << type << quint32(0);
    this->writePayload(stream);

    if (stream.status() == QDataStream::WriteFailed) {
        //throw StreamException("Unable to write on stream during a send op", type);
    }

}

void Packet::send(QWebSocket &m_webSocket) {
    if (!m_webSocket.isValid())
        return;
    // Create buffer and stream
    QByteArray qbuf;
    QDataStream stream(&qbuf, QIODevice::WriteOnly);

    // Define additional fields, serialize and send packets.
    this->serialize(qbuf, stream);

    // We set and write the size after the serialization because we dont know the qByteArray length in a previous moment
    this->setSize(qbuf.size());
    this->writeSize(stream);
//    qDebug() << "---";
//    qDebug() << "[GENERIC SEND]" << "Buf size:" << qbuf.size();;
    m_webSocket.sendBinaryMessage(qbuf);
    // Try to force a flush of the websocket buffer
    m_webSocket.flush();//"\xAF\x00\x00\x01\x00\x00\x00\x04\x00\x00\x00\b\x00t\x00""e\x00s\x00t"

}

quint32 Packet::getPayloadLen() {
    return getSize();
}

void Packet::read(QDataStream &stream) {

    readPayload(stream);
    if (stream.status() != QDataStream::Ok || !stream.atEnd()) {
        //throw StreamException("Unable to read from stream", type);
    }
}

quint8 Packet::getFlags() const {
    return flags;
}

void Packet::setFlags(quint8 flags) {
    Packet::flags = flags;
}

quint16 Packet::getType() const {
    return type;
}

void Packet::setType(quint16 type) {
    Packet::type = type;
}

quint32 Packet::getSize() const {
    return size;
}

void Packet::setSize(quint32 size) {
    Packet::size = size;
}

void Packet::writeSize(QDataStream &stream) {
    // Write at the right position
    stream.device()->seek(4);//
    //stream << (quint32)(size - 4 - sizeof(quint32));//Set just payload size
    stream << (quint32) (size - 4 - sizeof(quint32));// Set full size
}

const QByteArray &Packet::getData() const {
    return data;
}

void Packet::setData(const QByteArray &data) {
    Packet::data = data;
}

quint8 Packet::getSignature() const {
    return signature;
}

void Packet::setSignature(quint8 signature) {
    Packet::signature = signature;
}



/*** Packet buffer ***/
PacketBuffer::PacketBuffer()
        : mType(0), mSize(0) {
};

PacketBuffer::~PacketBuffer() {
};

void PacketBuffer::setType(quint16 t) {
    mType = t;
};

void PacketBuffer::setDataSize(quint32 s) {
    mSize = s;
};

quint16 PacketBuffer::getType() const {
    return mType;
};

quint32 PacketBuffer::getDataSize() const {
    return mSize;
};

quint32 PacketBuffer::getReceivedSize() const {
    return (quint32) buffer.size();
}

void PacketBuffer::append(QByteArray array) {
    buffer.append(array);
};

void PacketBuffer::clearBuffer() {
    mSize = 0;
    buffer.clear();
};

bool PacketBuffer::isComplete() {
//    qDebug() << mSize << getReceivedSize();
    return mSize == getReceivedSize();
};

QByteArray *PacketBuffer::bufferPtr() {
    return &buffer;
};

QDataStream &operator>>(QDataStream &in, PacketBuffer &PacketBuffer) {
    in >> PacketBuffer.mHeader >> PacketBuffer.mFlags >> PacketBuffer.mType >> PacketBuffer.mSize;
    return in;
}

/*** Packet forger **/

QSharedPointer<Packet> PacketBuilder::Container(quint8 type) {
    switch (type) {
        case PACK_TYPE_PING:
            return QSharedPointer<Packet>(new PingPacket());
        case PACK_TYPE_LOGIN_REQ:
            return QSharedPointer<Packet>(new class LoginReqPacket());
        case PACK_TYPE_LOGIN_OK:
            return QSharedPointer<Packet>(new LoginOkPacket());
        case PACK_TYPE_LOGOUT_REQ:
            return QSharedPointer<Packet>(new class LogoutReqPacket());

        case PACK_TYPE_ACC_CREATE:
            return QSharedPointer<Packet>(new class AccountCreationPacket());
        case PACK_TYPE_ACC_OK:
            return QSharedPointer<Packet>(new AccountOkPacket());
        case PACK_TYPE_ACC_UPDATE:
            return QSharedPointer<Packet>(new class AccountUpdatePacket());

        case PACK_TYPE_MSG:
            return QSharedPointer<Packet>(new class Message());
        case PACK_TYPE_BIGMSG:
            return QSharedPointer<Packet>(new class BigMessage());
        case PACK_TYPE_ALIGN:
            return QSharedPointer<Packet>(new class AlignMessage());

        case PACK_TYPE_CURSOR_POS:
            return QSharedPointer<Packet>(new class CursorPacket());

        case PACK_TYPE_DOC_CREATE:
            return QSharedPointer<Packet>(new class DocumentCreatePacket());
        case PACK_TYPE_DOC_OPEN:
            return QSharedPointer<Packet>(new class DocumentOpenPacket());
        case PACK_TYPE_DOC_DEL:
            return QSharedPointer<Packet>(new class DocumentDelPacket());
        case PACK_TYPE_DOC_OK:
            return QSharedPointer<Packet>(new class DocumentOkPacket());
        case PACK_TYPE_DOC_ASKSURI:
            return QSharedPointer<Packet>(new class DocumentAskSharableURIPacket());
        case PACK_TYPE_DOC_LIST:
            return QSharedPointer<Packet>(new class DocumentListPacket());
        case PACK_TYPE_DOC_USERLIST:
            return QSharedPointer<Packet>(new class DocumentBeaconOnlineUsers());
        case PACK_TYPE_DOC_CLOSE:
            return QSharedPointer<Packet>(new class DocumentClosePacket());

        default:
            throw UknownPacketException("[ERROR] Unknown packet.");
            break;
    }
}

QSharedPointer<Packet>  PacketBuilder::Ping(QString msg) {
    return QSharedPointer<Packet>(new PingPacket(msg));
}

QSharedPointer<Packet>  PacketBuilder::LoginReqPacket(QString username, QString hashedPassword) {
    return QSharedPointer<Packet>(new class LoginReqPacket(username, hashedPassword));
}

QSharedPointer<Packet>  PacketBuilder::LoginOk(User user) {
    return QSharedPointer<Packet>(new class LoginOkPacket(user));
}

QSharedPointer<Packet>  PacketBuilder::AccountCreationPacket(QString username, QString password, QString name, QString surname,
                                                   QImage profilePic) {
    return QSharedPointer<Packet>(new class AccountCreationPacket(username, password, name, surname, profilePic));
}

QSharedPointer<Packet>  PacketBuilder::AccountOk(User user) {
    return QSharedPointer<Packet>(new class AccountOkPacket(user));
}

QSharedPointer<Packet>  PacketBuilder::AccountUpdatePacket(int id, QString username, QString password, QString name, QString surname,
                                                 QImage profilePic) {
    return QSharedPointer<Packet>(new class AccountUpdatePacket(id, username, password, name, surname, profilePic));
}

QSharedPointer<Packet>  PacketBuilder::Message(int type, QSymbol qs, int siteId) {
    return QSharedPointer<Packet>(new class Message(type, qs, siteId));
}

QSharedPointer<Packet>  PacketBuilder::BigMessage(int type, QVector<QSymbol> qss, int siteId) {
    return QSharedPointer<Packet>(new class BigMessage(type, qss, siteId));
}

QSharedPointer<Packet>  PacketBuilder::AlignMessage(QSymbol positionStart, int delta, Qt::Alignment alignment, int siteId) {
    return QSharedPointer<Packet>(new class AlignMessage(positionStart, delta, alignment, siteId));
}

QSharedPointer<Packet>  PacketBuilder::CursorPacket(qint32 userId, qint32 newPosition) {
    return QSharedPointer<Packet>(new class CursorPacket(userId, newPosition));
}


QSharedPointer<Packet>  PacketBuilder::DocumentCreatePacket(QString docName, qint32 userId) {
    return QSharedPointer<Packet>(new class DocumentCreatePacket(docName, userId));
}

QSharedPointer<Packet>  PacketBuilder::DocumentOpenPacket(QString docName, qint32 userId) {
    return QSharedPointer<Packet>(new class DocumentOpenPacket(docName, userId));
}

QSharedPointer<Packet>  PacketBuilder::DocumentDelPacket(QString docName, qint32 userId) {
    return QSharedPointer<Packet>(new class DocumentDelPacket(docName, userId));
}

QSharedPointer<Packet>  PacketBuilder::DocumentOkPacket(QString docName, qint32 userId, QVector<QVector<QSymbol>> qsymbols) {
    return QSharedPointer<Packet>(new class DocumentOkPacket(userId, docName, qsymbols));
}

QSharedPointer<Packet>  PacketBuilder::DocumentAskSharableURIPacket(int docId, qint32 userId, QString sharableURI) {
    return QSharedPointer<Packet>(new class DocumentAskSharableURIPacket(docId, userId, sharableURI));
}

QSharedPointer<Packet>  PacketBuilder::DocumentListPacket(qint32 userId, QVector<QString> docList) {
    return QSharedPointer<Packet>(new class DocumentListPacket(userId, docList));
}

QSharedPointer<Packet>  PacketBuilder::DocumentBeaconOnlineUsers(QVector<User> onlineUserList, qint32 docId, QVector<User> completeUserList) {
    return QSharedPointer<Packet>(new class DocumentBeaconOnlineUsers(onlineUserList, docId, completeUserList));
}


UknownPacketException::UknownPacketException(char const* const message) throw()
    : std::runtime_error(message)
{

}
