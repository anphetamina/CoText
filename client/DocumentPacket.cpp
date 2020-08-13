//
// Created by Emanuele Munafò on 22/04/2020.
//

#include "PacketDef.h"
#include "DocumentPacket.h"

/** Document Creation packet **/
DocumentCreatePacket::DocumentCreatePacket(): Packet(PACK_TYPE_DOC_CREATE){}
DocumentCreatePacket::DocumentCreatePacket(qint32 userId, QString docName): Packet(PACK_TYPE_DOC_CREATE), userId(userId), docName(docName){};

qint32 DocumentCreatePacket::getuserId() const {
    return userId;
}

QString DocumentCreatePacket::getdocName() const {
    return docName;
}

void DocumentCreatePacket::writePayload(QDataStream& stream) const
{
    stream << userId << docName;
}

void DocumentCreatePacket::readPayload(QDataStream& stream)
{
    stream >> userId >> docName;
}

/** Document Open packet **/
// Inherit from a DocumentBaseActionClass?
DocumentOpenPacket::DocumentOpenPacket(): Packet(PACK_TYPE_DOC_OPEN){}
DocumentOpenPacket::DocumentOpenPacket(qint32 userId, QString docName): Packet(PACK_TYPE_DOC_OPEN), userId(userId), docName(docName){};

qint32 DocumentOpenPacket::getuserId() const {
    return userId;
}

QString DocumentOpenPacket::getdocName() const {
    return docName;
}

void DocumentOpenPacket::writePayload(QDataStream& stream) const
{
    stream << userId << docName;
}

void DocumentOpenPacket::readPayload(QDataStream& stream)
{
    stream >> userId >> docName;
}


/** Delete Document packet **/
// Inherit from a DocumentBaseActionClass?
DocumentDelPacket::DocumentDelPacket(): Packet(PACK_TYPE_DOC_DEL){}
DocumentDelPacket::DocumentDelPacket(qint32 userId, QString docName): Packet(PACK_TYPE_DOC_DEL), userId(userId), docName(docName){};

qint32 DocumentDelPacket::getuserId() const {
    return userId;
}

QString DocumentDelPacket::getdocName() const {
    return docName;
}

void DocumentDelPacket::writePayload(QDataStream& stream) const
{
    stream << userId << docName;
}

void DocumentDelPacket::readPayload(QDataStream& stream)
{
    stream >> userId >> docName;
}

/** Document Ok packet **/
// Inherit from a DocumentBaseActionClass?
DocumentOkPacket::DocumentOkPacket(): Packet(PACK_TYPE_DOC_OK){}
DocumentOkPacket::DocumentOkPacket(qint32 userId, QString docName): Packet(PACK_TYPE_DOC_OK), userId(userId), docName(docName){};

qint32 DocumentOkPacket::getuserId() const {
    return userId;
}

QString DocumentOkPacket::getdocName() const {
    return docName;
}

void DocumentOkPacket::writePayload(QDataStream& stream) const
{
    stream << userId << docName;
}

void DocumentOkPacket::readPayload(QDataStream& stream)
{
    stream >> userId >> docName;
}

/** DocumentAskSharableURIPacket packet **/
// Inherit from a DocumentBaseActionClass?
DocumentAskSharableURIPacket::DocumentAskSharableURIPacket(): Packet(PACK_TYPE_DOC_ASKSURI){}
DocumentAskSharableURIPacket::DocumentAskSharableURIPacket(qint32 userId, QString docName, QString sharableURI ): Packet(PACK_TYPE_DOC_ASKSURI), userId(userId), docName(docName), sharableURI(sharableURI){};

qint32 DocumentAskSharableURIPacket::getuserId() const {
    return userId;
}

QString DocumentAskSharableURIPacket::getdocName() const {
    return docName;
}

void DocumentAskSharableURIPacket::writePayload(QDataStream& stream) const
{
    stream << userId << docName << sharableURI;
}

void DocumentAskSharableURIPacket::readPayload(QDataStream& stream)
{
    stream >> userId >> docName << sharableURI;
}


/*
static PacketHandler DocumentCreation(QString docName);
static PacketHandler DocumentRemoval(QString docURI);
static PacketHandler DocumentOpen(QString docURI);
static PacketHandler DocumentClose();
static PacketHandler DocumentErr(QString error);

*/