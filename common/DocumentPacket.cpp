//
// Created by Emanuele Munafò on 22/04/2020.
//

#include "PacketDef.h"
#include "DocumentPacket.h"

/** Document Creation packet **/
DocumentCreatePacket::DocumentCreatePacket(): Packet(PACK_TYPE_DOC_CREATE){}
DocumentCreatePacket::DocumentCreatePacket(QString docName, qint32 userId ): Packet(PACK_TYPE_DOC_CREATE), userId(userId), docName(docName){};

int DocumentCreatePacket::getuserId() const {
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

/** Document ask list packet **/
DocumentAskListPacket::DocumentAskListPacket(): Packet(PACK_TYPE_DOC_ASKLIST){}
DocumentAskListPacket::DocumentAskListPacket(qint32 userId): Packet(PACK_TYPE_DOC_ASKLIST), userId(userId){};

int DocumentAskListPacket::getuserId() const {
    return userId;
}

void DocumentAskListPacket::writePayload(QDataStream& stream) const
{
    stream << userId ;
}

void DocumentAskListPacket::readPayload(QDataStream& stream)
{
    stream >> userId;
}

/** Document list packet.  response for DocumentAskListPacket. Qvector of len 0 on failure/no document available **/
DocumentListPacket::DocumentListPacket(): Packet(PACK_TYPE_DOC_LIST){}
DocumentListPacket::DocumentListPacket(qint32 userId, QVector<QString> docList): Packet(PACK_TYPE_DOC_LIST), userId(userId), docList(docList){};

int DocumentListPacket::getuserId() const {
    return userId;
}
QVector<QString> DocumentListPacket::getdocList() const {
    return docList;
}

void DocumentListPacket::writePayload(QDataStream& stream) const
{
    stream << userId << docList ;
}

void DocumentListPacket::readPayload(QDataStream& stream)
{
    stream >> userId >> docList;
}



/** Document Open packet **/
// Inherit from a DocumentBaseActionClass?
DocumentOpenPacket::DocumentOpenPacket(): Packet(PACK_TYPE_DOC_OPEN){}
DocumentOpenPacket::DocumentOpenPacket(QString docName, qint32 userId ): Packet(PACK_TYPE_DOC_OPEN), userId(userId), docName(docName){};

int DocumentOpenPacket::getuserId() const {
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
DocumentDelPacket::DocumentDelPacket(QString docName, qint32 userId ): Packet(PACK_TYPE_DOC_DEL), userId(userId), docName(docName){};

int DocumentDelPacket::getuserId() const {
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
DocumentOkPacket::DocumentOkPacket(int docId, QString docName, QVector<QVector<QSymbol>> qsymbols ): Packet(PACK_TYPE_DOC_OK), docId(docId), docName(docName), qsymbols(qsymbols){};
//DocumentOkPacket::DocumentOkPacket(int docId, QString docName, int siteId, std::vector<std::vector<Symbol>> symbols, std::vector<QTextCharFormat> vcf ): Packet(PACK_TYPE_DOC_OK), docId(docId), siteId(siteId), docName(docName), qsymbols(toQVector(symbols)){};



int DocumentOkPacket::getdocId() const {
    return docId;
}

QString DocumentOkPacket::getdocName() const {
    return docName;
}

QVector<QVector<QSymbol>> DocumentOkPacket::getqsymbols() const {
    return qsymbols;
}

std::vector<std::vector<QSymbol>>  DocumentOkPacket::getsymbols() const {
    return toVector(qsymbols);
}

void DocumentOkPacket::writePayload(QDataStream& stream) const
{
    stream << docId << docName << qsymbols;// <<  qsymbols;
}

void DocumentOkPacket::readPayload(QDataStream& stream)
{
    stream >> docId >> docName >> qsymbols;// >>  qsymbols;
}

/** DocumentAskSharableURIPacket packet **/
// Inherit from a DocumentBaseActionClass?
DocumentAskSharableURIPacket::DocumentAskSharableURIPacket(): Packet(PACK_TYPE_DOC_ASKSURI){}
DocumentAskSharableURIPacket::DocumentAskSharableURIPacket(int docId, qint32 userId, QString sharableURI ): Packet(PACK_TYPE_DOC_ASKSURI), userId(userId), docId(docId), sharableURI(sharableURI){};

int DocumentAskSharableURIPacket::getuserId() const {
    return userId;
}

int DocumentAskSharableURIPacket::getdocId() const {
    return docId;
}

QString DocumentAskSharableURIPacket::getURI() const{
    return sharableURI;
}

void DocumentAskSharableURIPacket::writePayload(QDataStream& stream) const
{
    stream << userId << docId << sharableURI;
}

void DocumentAskSharableURIPacket::readPayload(QDataStream& stream)
{
    stream >> userId >> docId >> sharableURI;
}

/** DocumentBeaconOnlineUsers packet **/
// Inherit from a DocumentBaseActionClass?
DocumentBeaconOnlineUsers::DocumentBeaconOnlineUsers(): Packet(PACK_TYPE_DOC_USERLIST){}
DocumentBeaconOnlineUsers::DocumentBeaconOnlineUsers(QVector<User> userList, qint32 docId): Packet(PACK_TYPE_DOC_USERLIST), userList(userList), docId(docId){};

int DocumentBeaconOnlineUsers::getdocId() const {
    return docId;
}

QVector<User> DocumentBeaconOnlineUsers::getuserList() const {
    return userList;
}

void DocumentBeaconOnlineUsers::writePayload(QDataStream& stream) const
{
    stream << userList << docId;
}

void DocumentBeaconOnlineUsers::readPayload(QDataStream& stream)
{
    stream >> userList >> docId;
}

//QVector::fromStdVector ( const std::vector<T> & vector )
