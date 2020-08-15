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


/** Packet **/
Packet::Packet(uint16_t type) : header(0xAF), type(type), flags(0x00)
{}

Packet::~Packet()
{}

// Serialize the content of the packet
// Could use QDataStream
void Packet::serialize(QByteArray buf, QDataStream& stream)
{
    // Header|FLAGS|type|size|payload

    stream << header << flags << type << quint32(0);
    this->writePayload(stream);

    if (stream.status() == QDataStream::WriteFailed) {
        //throw StreamException("Unable to write on stream during a send op", type);
    }

}

void Packet::send(QWebSocket& m_webSocket) {
    if (!m_webSocket.isValid())
        return;
    // Other check if missing fields? TODO:check later
    // Create buffer and stream
    QByteArray qbuf;
    QDataStream stream(&qbuf, QIODevice::WriteOnly);

    // Define additional fields, serialize and send packets.
    this->serialize(qbuf, stream);

    // We set and write the size after the serialization because we dont know the qByteArray length in a previous moment
    this->setSize(qbuf.size());
    this->writeSize(stream);
    qDebug() << "Buf size:" << qbuf.size();
    qDebug() << "A packet was just sent";
    m_webSocket.sendBinaryMessage(qbuf);
    // Try to force a flush of the websocket buffer
    m_webSocket.flush();//"\xAF\x00\x00\x01\x00\x00\x00\x04\x00\x00\x00\b\x00t\x00""e\x00s\x00t"

}

quint32 Packet::getPayloadLen(){
    return getSize();
}

void Packet::read(QDataStream& stream){

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
void Packet::writeSize(QDataStream& stream) {
    // Write at the right position
    stream.device()->seek(4);//
    //stream << (quint32)(size - 4 - sizeof(quint32));//Set just payload size
    stream << (quint32)(size - 4 - sizeof(quint32));// Set full size
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


/** PAcket handler**/
PacketHandler::PacketHandler()
        : ptr(nullptr), ref(nullptr)
{
}

PacketHandler::PacketHandler(std::nullptr_t)
        : ptr(nullptr), ref(nullptr)
{
}

PacketHandler::PacketHandler(Packet* p)
try
        : ptr(p), ref(new int(1))
{
}
catch (...)
{
    delete p;
    throw;
}

PacketHandler::PacketHandler(const PacketHandler& other)
        : ptr(other.ptr), ref(other.ref)
{
    if (ptr != nullptr)
        ++(*ref);
}

PacketHandler::PacketHandler(PacketHandler&& other) noexcept
        : ptr(other.ptr), ref(other.ref)
{
    other.ptr = nullptr;
}

PacketHandler& PacketHandler::operator=(PacketHandler other)
{
    // Copy & Swap assignment operator implementation
    std::swap(this->ptr, other.ptr);
    std::swap(this->ref, other.ref);
    return *this;
}

Packet& PacketHandler::operator*() const
{
    return *ptr;
}

Packet* PacketHandler::operator->() const
{
    return ptr;
}

PacketHandler::operator bool() const
{
    return ptr != nullptr;
}

Packet* PacketHandler::get() const
{
    return ptr;
}

void PacketHandler::reset()
{
    if (ptr != nullptr)
    {
        --(*ref);
        if (*ref == 0)
        {
            delete ptr;
            delete ref;
        }
    }

    ptr = nullptr;
    ref = nullptr;
}

PacketHandler::~PacketHandler()
{
    if (ptr != nullptr)
    {
        --(*ref);
        if (*ref == 0)
        {
            delete ptr;
            delete ref;
        }
    }
}


/*** Packet buffer ***/


PacketBuffer::PacketBuffer()
        : mType(0), mSize(0)
{
};

PacketBuffer::~PacketBuffer()
{
};

void PacketBuffer::setType(quint16 t)
{
    mType = t;
};

void PacketBuffer::setDataSize(quint32 s)
{
    mSize = s;
};

quint16 PacketBuffer::getType() const
{
    return mType;
};

quint32 PacketBuffer::getDataSize() const
{
    return mSize;
};

quint32 PacketBuffer::getReceivedSize() const
{
    return (quint32)buffer.size();
}

void PacketBuffer::append(QByteArray array)
{
    buffer.append(array);
};

void PacketBuffer::clearBuffer()
{
    mSize = 0; buffer.clear();
};

bool PacketBuffer::isComplete()
{
    qDebug() << mSize << getReceivedSize();
    return mSize == getReceivedSize();
};

QByteArray* PacketBuffer::bufferPtr()
{
    return &buffer;
};

QDataStream& operator>>(QDataStream& in, PacketBuffer& PacketBuffer)
{
    in >>PacketBuffer.mHeader  >>PacketBuffer.mFlags >> PacketBuffer.mType >> PacketBuffer.mSize;
    return in;
}



/*** Packet forger **/

PacketHandler PacketBuilder::Container(quint8 type)
{
    switch (type)
    {
        case PACK_TYPE_PING:			    return new PingPacket();
        case PACK_TYPE_LOGIN_REQ:			return new class LoginReqPacket();
        case PACK_TYPE_LOGIN_OK:			return new LoginOkPacket();
        case PACK_TYPE_LOGOUT_REQ:			return new class LogoutReqPacket();

        case PACK_TYPE_ACC_CREATE:			return new class AccountCreationPacket();
        case PACK_TYPE_ACC_OK:			    return new AccountOkPacket();
        case PACK_TYPE_ACC_UPDATE:			return new class AccountUpdatePacket();
        case PACK_TYPE_MSG:			        return new class Message();
        
        default:
            throw std::exception();//TODO: create custom exception
            break;
    }
}


PacketHandler PacketBuilder::Ping(QString msg)
{
    return new PingPacket(msg);
}

PacketHandler PacketBuilder::LoginReqPacket(QString username, QString hashedPassword)
{
    return new class LoginReqPacket(username, hashedPassword);
}

PacketHandler PacketBuilder::LoginOk(User user)
{
    return new class LoginOkPacket(user);
}
PacketHandler PacketBuilder::AccountCreationPacket(QString username,QString password, QString name, QString surname, QIcon profilePic)
{
    return new class AccountCreationPacket(username, password, name, surname, profilePic);
}
PacketHandler PacketBuilder::AccountOk(User user)
{
    return new class AccountOkPacket(user);
}

PacketHandler PacketBuilder::AccountUpdatePacket(QString username,QString password, QString name, QString surname, QIcon profilePic)
{
    return new class AccountUpdatePacket(username, password, name, surname, profilePic);
}

PacketHandler PacketBuilder::Message(int type, QSymbol qs, int siteId)
{
    return new class Message(type, qs, siteId);
}

PacketHandler PacketBuilder::CursorPacket(qint32 userId, qint32 newPosition)
{
    return new class CursorPacket(userId, newPosition);
}

/*
 * Add new PacketBuilder method to create the empty packet (-> add def to Packet.cpp and PAcket.h and add #include newpacket.h in both file)
 * Create and define new code for packet in PacketDef.h
 * Add case in switch in the  PacketHandler PacketBuilder::Container(quint8 type) method. (Packet.cpp)
 * Implement operator<< and operator>> in the below class if not available so that writePayload and ReadPayload can serialize the payload of packet.
 *
 */