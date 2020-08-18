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
    DocumentCreatePacket(QString docName, qint32 userId);
    ~DocumentCreatePacket() {};
    int getuserId() const;
    QString getdocName() const;
};

/** Document ask list packet **/
class DocumentAskListPacket : public Packet {
    friend PacketBuilder;
private:
    qint32 userId;
protected:
DocumentAskListPacket();

    void writePayload(QDataStream& stream) const override;
    void readPayload(QDataStream& stream) override;

public:
    DocumentAskListPacket(qint32 userId);
    ~DocumentAskListPacket() {};
    int getuserId() const;
};

/** Document list packet **/
class DocumentListPacket : public Packet {
    friend PacketBuilder;
private:
    qint32 userId;
    QVector<QString> docList;
protected:
    DocumentListPacket();

    void writePayload(QDataStream& stream) const override;
    void readPayload(QDataStream& stream) override;

public:
    DocumentListPacket(qint32 userId, QVector<QString> docList);
    ~DocumentListPacket() {};
    int getuserId() const;
    QVector<QString> getdocList() const;
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
    DocumentOpenPacket(QString docName, qint32 userId);
    ~DocumentOpenPacket() {};
    int getuserId() const;
    QString getdocName() const;
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
    DocumentDelPacket(QString docName, qint32 userId);
    ~DocumentDelPacket() {};
    int getuserId() const;
    QString getdocName() const;
};

/** Document Ok packet: the desirable response for a DocumentOpen packet **/
//TODO: Add attribute for whatever will be used to exchange the whole document in bulk
class DocumentOkPacket : public Packet {

    friend PacketBuilder;
private:
    int docId;
    QString docName;

    //int siteId;
    QVector<QVector<QSymbol>> qsymbols;
protected:
    DocumentOkPacket();

    void writePayload(QDataStream& stream) const override;
    void readPayload(QDataStream& stream) override;

public:
    DocumentOkPacket(int docId, QString docName, QVector<QVector<QSymbol>> qsymbols);
    ~DocumentOkPacket() {};
    //int getsiteId() const;
    int getdocId() const;
    QString getdocName() const;

    QVector<QVector<QSymbol>> getqsymbols() const;

    std::vector<std::vector<Symbol>> getsymbols() const;
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
    DocumentAskSharableURIPacket(QString docName, qint32 userId, QString sharableURI);
    ~DocumentAskSharableURIPacket() {};
    int getuserId() const;
    QString getdocName() const;

};

std::vector<std::vector<Symbol>> toVector(QVector<QVector<QSymbol>> qsymbols);
QVector<QTextCharFormat> toCFVector(QVector<QVector<QSymbol>> qsymbols);