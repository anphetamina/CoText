//
// Created by Emanuele Munafò on 13/04/2020.
//

#pragma once

#include <cstdint>
#include <vector>
#include <QByteArray>
#include <QWebSocket>
#include <QDataStream>
#include "User.h"
#include "QSymbol.h"


class Packet
{
public:
    Packet();

    Packet(uint16_t type);

    quint8 getFlags() const;

    void setFlags(quint8 flags);

    quint16 getType() const;

    void setType(quint16 type);

    quint32 getSize() const;

    void setSize(quint32 size);

    const QByteArray &getData() const;

    void setData(const QByteArray &data);

    quint8 getSignature() const;

    void setSignature(quint8 signature);

    virtual ~Packet();

    /*
    Serializes the packet to send over network .
    Fields are written to buffer already in network byte order (big-endian).
    */
    void serialize(std::vector<uint8_t>& buf);

protected:
    quint8 header = 0xAF; // Identify the beginning of toghedit packet`
    quint8 flags = 0x00; // Custom bitmask flags.
    quint16 type = 0x0000; // Identifies the packet's type - what is it used for (composed by 2 bytes)
    quint32 size = 0x00000000; // Size of `data`.
    QByteArray data; // Custom payload
    quint8 signature = 0x00; // Small signature (should be much and much bigger for a good security)
    //uint8_t checksum = 0x00; // Eventually add CRC or some error detection or correction system (not needed here, tcp is used)

public:


    void send();
    // Send the already defined packet
    void send(QWebSocket& m_webSocket);
    void read(QDataStream& stream);

    // Serialize using QDataStream implementation
    void serialize(QByteArray buf, QDataStream& stream);
    quint32 getPayloadLen();

protected:

    // Write the payload of a specific packet on the Qdatastream buffer
    virtual void writePayload(QDataStream& stream) const = 0;
    // Read the payload from the Qdatastream buffer
    virtual void readPayload(QDataStream& stream) = 0;

    void writeSize(QDataStream &stream);
};


class PacketBuffer
{
    friend QDataStream& operator>>(QDataStream& in, PacketBuffer& packetBuffer);		// Allow access to deserialization method

private:
    quint8 mHeader;
    quint8 mFlags;
    quint16 mType;
    quint32 mSize;
    QByteArray buffer;

public:
    PacketBuffer();
    ~PacketBuffer();

    void append(QByteArray array);
    void clearBuffer();
    bool isComplete();
    QByteArray* bufferPtr();

    /* setter */
    void setType(quint16 t);
    void setDataSize(quint32 s);

    /* getter */
    quint16 getType() const;
    quint32 getDataSize() const;
    quint32 getReceivedSize() const;
};


class PacketBuilder
{
public:

    static QSharedPointer<Packet>  Container(quint8 type);
    static QSharedPointer<Packet>  Ping(QString msg);
    static QSharedPointer<Packet>  LoginReqPacket(QString username, QString hashedPassword);
    static QSharedPointer<Packet>  LoginOk(User user);
    static QSharedPointer<Packet>  LogoutReq();

    static QSharedPointer<Packet>  AccountCreationPacket(QString username,QString password, QString name, QString surname, QImage profilePic);
    static QSharedPointer<Packet>  AccountOk(User user);
    static QSharedPointer<Packet>  AccountUpdatePacket(int id, QString username,QString password, QString name, QString surname, QImage profilePic);

    static QSharedPointer<Packet>  Message(int type, QSymbol qs, int siteId);
    static QSharedPointer<Packet>  BigMessage(int type, QVector<QSymbol> qss, int siteId);
    static QSharedPointer<Packet>  AlignMessage(QSymbol positionStart, int delta, Qt::Alignment alignment, int siteId);

    static QSharedPointer<Packet>  CursorPacket(qint32 userId, qint32 newPosition);

    static QSharedPointer<Packet>  DocumentCreatePacket(QString docName, qint32 userId);
    static QSharedPointer<Packet>  DocumentOpenPacket(QString docName, qint32 userId);
    static QSharedPointer<Packet>  DocumentDelPacket(QString docName, qint32 userId);
    static QSharedPointer<Packet>  DocumentOkPacket(QString docName, qint32 userId, QVector<QVector<QSymbol>> qsymbols);
    static QSharedPointer<Packet>  DocumentAskSharableURIPacket(int docId, qint32 userId, QString sharableURI);
    static QSharedPointer<Packet>  DocumentListPacket(qint32 userId, QVector<QString> docList);
    static QSharedPointer<Packet>  DocumentClosePacket();

    QSharedPointer<Packet>  DocumentBeaconOnlineUsers(QVector<User> onlineUserList, qint32 docId, QVector<User> completeUserList);

};


class UknownPacketException : public std::runtime_error
{
    std::string what_message;
public:
    UknownPacketException(char const* const message) throw();
    //virtual char const* what() const throw();
};