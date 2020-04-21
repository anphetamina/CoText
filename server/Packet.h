//
// Created by Emanuele Munafò on 13/04/2020.
//

#pragma once

#include <cstdint>
#include <vector>
#include <QByteArray>
#include <QWebSocket>
#include <QDataStream>

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

class PacketHandler
{
private:

    Packet* ptr;
    int* ref;

public:

    PacketHandler();
    PacketHandler(std::nullptr_t);
    PacketHandler(Packet* m);

    PacketHandler(const PacketHandler& other);
    PacketHandler(PacketHandler&& other) noexcept;

    PacketHandler& operator=(PacketHandler other);

    Packet& operator*() const;
    Packet* operator->() const;

    operator bool() const;

    Packet* get() const;
    void reset();

    ~PacketHandler();
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

    static PacketHandler Container(quint8 type);
    static PacketHandler Ping(QString msg);
    /*

    static PacketHandler AccountCreation(QString username, QImage icon, QString password);
    static PacketHandler AccountUpdate(QImage icon, QString password);
    static PacketHandler AccountConfirmation(User user);
    static PacketHandler AccountErr(QString error);

    static PacketHandler LoginReq(QString username);
    static PacketHandler LoginOk(User user);
    static PacketHandler LoginErr(QString error);
    static PacketHandler Logout();

    static PacketHandler DocumentCreation(QString docName);
    static PacketHandler DocumentRemoval(QString docURI);
    static PacketHandler DocumentOpen(QString docURI);
    static PacketHandler DocumentDismissal();
    static PacketHandler DocumentClose();
    static PacketHandler DocumentErr(QString error);

    static PacketHandler CharsInsert(QVector<Symbol> symbols, bool isLast, TextBlockID bId, QTextBlockFormat blkFmt);
    static PacketHandler CharsDelete(QVector<Position> positions);
    static PacketHandler CursorSeek(qint32 userId, qint32 newPosition);

    static PacketHandler PresenceRefresh(qint32 userId, QString nickname, QImage icon);
    static PacketHandler PresenceRegister(qint32 userId, QString nickname, QImage icon);
    static PacketHandler PresenceUnregister(qint32 userId);

    static PacketHandler Error(QString error);*/
};