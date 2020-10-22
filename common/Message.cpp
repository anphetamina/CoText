#include "Message.h"
#include "PacketDef.h"
#include "QSymbol.h"

//Message::Message(int type, const Symbol s, int siteId) : type(type), s(s), siteId(siteId) {}
Message::Message() : Packet(PACK_TYPE_MSG) {}

Message::Message(int type, QSymbol qs, int siteId) : Packet(PACK_TYPE_MSG), type(type), qs(qs), siteId(siteId) {};

int Message::getType() const {
    return type;
}

QSymbol Message::getQS() const {
    return qs;
}

int Message::getSiteId() const {
    return siteId;
}

void Message::writePayload(QDataStream &stream) const {
    stream << type << qs << siteId;
}

void Message::readPayload(QDataStream &stream) {
    stream >> type >> qs >> siteId;
}

// Packet to transmit multiple Qsymbol
BigMessage::BigMessage() : Packet(PACK_TYPE_BIGMSG) {}

BigMessage::BigMessage(int type, QVector<QSymbol> qss, int siteId) : Packet(PACK_TYPE_BIGMSG), type(type), qss(qss),
                                                                     siteId(siteId) {};

int BigMessage::getType() const {
    return type;
}

QVector<QSymbol> BigMessage::getQSS() const {
    return qss;
}

int BigMessage::getSiteId() const {
    return siteId;
}

void BigMessage::writePayload(QDataStream &stream) const {
    stream << type << qss << siteId;
}

void BigMessage::readPayload(QDataStream &stream) {
    stream >> type >> qss >> siteId;
}

// Packet to transmit multiple Qsymbol
AlignMessage::AlignMessage() : Packet(PACK_TYPE_ALIGN) {}

AlignMessage::AlignMessage(QSymbol positionStart, int delta, Qt::Alignment alignment, int siteId) : Packet(PACK_TYPE_ALIGN),
                                                                                                positionStart(
                                                                                                        positionStart),
                                                                                                delta(delta),
                                                                                                alignment(alignment),
                                                                                                siteId(siteId) {};

QSymbol AlignMessage::getPositionStart() const {
    return positionStart;
}

int AlignMessage::getDelta() const {
    return delta;
}

Qt::Alignment AlignMessage::getAlignment() const {
    return alignment;
}

int AlignMessage::getSiteId() const {
    return siteId;
}

void AlignMessage::writePayload(QDataStream &stream) const {
    stream << positionStart << delta << alignment << siteId;
}

void AlignMessage::readPayload(QDataStream &stream) {
    stream >> positionStart >> delta >> alignment >> siteId;
}

QDataStream &operator>>(QDataStream &in, AlignMessage &am) {
    in >> am.positionStart >> am.delta >> am.alignment >> am.siteId;
    return in;
}

QDataStream &operator<<(QDataStream &out, const AlignMessage &am) {
    out << am.positionStart << am.delta << am.alignment << am.siteId;
    return out;
}