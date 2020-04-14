#include "Message.h"

Message::Message(int type, const Symbol s, int siteId) : type(type), s(s), siteId(siteId) {}

int Message::getType() const {
    return type;
}

const Symbol &Message::getS() const {
    return s;
}

int Message::getSiteId() const {
    return siteId;
}

