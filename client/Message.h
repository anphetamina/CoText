#ifndef PDS_CPP_MESSAGE_H
#define PDS_CPP_MESSAGE_H


#include "Symbol.h"

class Message {
private:
    int type;
    Symbol s;
    std::string siteId;
public:
    Message(int type, const Symbol s, std::string siteId);

    int getType() const;

    const Symbol &getS() const;

    std::string getSiteId() const;

    int getCounter() const;
};


#endif //PDS_CPP_MESSAGE_H
