#include "Message.h"

Message::Message(int type, const Symbol s, std::string siteId) : type(type), s(s), siteId(siteId) {}

int Message::getType() const {
    return type;
}

const Symbol &Message::getS() const {
    return s;
}

std::string Message::getSiteId() const {
    return siteId;
}

