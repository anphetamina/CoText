#include "Message.h"
#include "PacketDef.h"
#include "QSymbol.h"

//Message::Message(int type, const Symbol s, int siteId) : type(type), s(s), siteId(siteId) {}
Message::Message(): Packet(PACK_TYPE_MSG){}
Message::Message(int type, QSymbol qs, int siteId): Packet(PACK_TYPE_MSG), type(type), qs(qs), siteId(siteId){};
//Message::Message(int type, Symbol s, int siteId): Packet(PACK_TYPE_MSG), type(type), qs(s.toSerializable()), siteId(siteId){};

int Message::getType() const {
    return type;
}

Symbol Message::getS() const {
    return qs.toOriginal();
}
QSymbol Message::getQS() const {
    return qs;
}

int Message::getSiteId() const {
    return siteId;
}

void Message::writePayload(QDataStream& stream) const
{
    stream << type << qs << siteId;
}

void Message::readPayload(QDataStream& stream)
{
    stream >> type >> qs >> siteId;
}

