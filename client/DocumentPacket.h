//
// Created by Emanuele Munafò on 17/04/2020.
//

#pragma once

#include "Packet.h"
#include "PacketDef.h"

/** Document creation packet **/
class DocumentCreatePacket : public Packet {
    DocumentCreatePacket(QString docName, qint32 userId);

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
    int getuserId() const;
    QString getdocName() const;
};

/** Document open packet **/
class DocumentOpenPacket : public Packet {
    DocumentOpenPacket(QString docName, qint32 userId);

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
    int getuserId() const;
    QString getdocName() const;
};

/** Document delete packet **/
class DocumentDelPacket : public Packet {
    DocumentDelPacket(QString docName, qint32 userId);

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
    int getuserId() const;
    QString getdocName() const;
};

/** Document Ok packet: the desirable response for a DocumentOpen packet **/
//TODO: Add attribute for whatever will be used to exchange the whole document in bulk
class DocumentOkPacket : public Packet {
    DocumentOkPacket(int docId, QString docName, int siteId, std::vector<std::vector<Symbol>> symbols);

    friend PacketBuilder;
private:
    int docId;
    QString docName;

    int siteId;
    QVector<QVector<QSymbol>> qsymbols;
protected:
    DocumentOkPacket();

    void writePayload(QDataStream& stream) const override;
    void readPayload(QDataStream& stream) override;

public:
    DocumentOkPacket(int docId, QString docName, int siteId, QVector<QVector<QSymbol>> qsymbols);
    ~DocumentOkPacket() {};
    int getsiteId() const;
    int getdocId() const;
    QString getdocName() const;

    QVector<QVector<QSymbol>> getqsymbols() const;

    std::vector<std::vector<Symbol>> getsymbols() const;
};

/** DocumentAskSharableURIPacket: the  packet used to check permission and get the URI used to invite people**/
class DocumentAskSharableURIPacket : public Packet {
    DocumentAskSharableURIPacket(QString docName, qint32 userId, QString sharableURI);

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
    int getuserId() const;
    QString getdocName() const;

};

QVector<QVector<QSymbol>> toQVector(std::vector<std::vector<Symbol>> symbols);
std::vector<std::vector<Symbol>> toVector(QVector<QVector<QSymbol>> qsymbols);
