//
// Created by Emanuele Munafò on 17/04/2020.
//

#pragma once

#include "Packet.h"
#include "PacketDef.h"

/** Document creation packet **/
class DocumentCreatePacket : public Packet {
    friend PacketBuilder;
private:
    qint32 userId;
    QString docName;
protected:
    DocumentCreatePacket();

    void writePayload(QDataStream& stream) const override;
    void readPayload(QDataStream& stream) override;

public:
    DocumentCreatePacket(qint32 userId, QString docName);
    ~DocumentCreatePacket() {};
    QString getuserId() const;
    QString getdocName const;
};

/** Document open packet **/
class DocumentOpenPacket : public Packet {
    friend PacketBuilder;
private:
    qint32 userId;
    QString docName;
protected:
    DocumentOpenPacket();

    void writePayload(QDataStream& stream) const override;
    void readPayload(QDataStream& stream) override;

public:
    DocumentOpenPacket(qint32 userId, QString docName);
    ~DocumentOpenPacket() {};
    QString getuserId() const;
    QString getdocName const;
};

/** Document delete packet **/
class DocumentDelPacket : public Packet {
    friend PacketBuilder;
private:
    qint32 userId;
    QString docName;
protected:
    DocumentDelPacket();

    void writePayload(QDataStream& stream) const override;
    void readPayload(QDataStream& stream) override;

public:
    DocumentDelPacket(qint32 userId, QString docName);
    ~DocumentDelPacket() {};
    QString getuserId() const;
    QString getdocName const;
};

/** Document Ok packet: the desirable response for a DocumentOpen packet **/
//TODO: Add attribute for whatever will be used to exchange the whole document in bulk
class DocumentOkPacket : public Packet {
    friend PacketBuilder;
private:
    qint32 userId;
    QString docName;
protected:
    DocumentOkPacket();

    void writePayload(QDataStream& stream) const override;
    void readPayload(QDataStream& stream) override;

public:
    DocumentOkPacket(qint32 userId, QString docName);
    ~DocumentOkPacket() {};
    QString getuserId() const;
    QString getdocName const;
};

/** DocumentAskSharableURIPacket: the  packet used to check permission and get the URI used to invite people**/
class DocumentAskSharableURIPacket : public Packet {
    friend PacketBuilder;
private:
    qint32 userId;
    QString docName;
    QString sharableURI;

protected:
    DocumentAskSharableURIPacket();

    void writePayload(QDataStream& stream) const override;
    void readPayload(QDataStream& stream) override;

public:
    DocumentAskSharableURIPacket(qint32 userId, QString docName, QString sharableURI);
    ~DocumentAskSharableURIPacket() {};
    QString getuserId() const;
    QString getdocName const;
};
