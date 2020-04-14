#ifndef PDS_CPP_MESSAGE_H
#define PDS_CPP_MESSAGE_H


#include "Symbol.h"

class Message {
private:
    int type;
    Symbol s;
    int siteId;
public:
    Message(int type, const Symbol s, int siteId);

    int getType() const;

    const Symbol &getS() const;

    int getSiteId() const;

    int getCounter() const;
};


#endif //PDS_CPP_MESSAGE_H
