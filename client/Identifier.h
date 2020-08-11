//
// Created by asant on 28/03/2020.
//

#ifndef CRDTPP_MULTI_IDENTIFIER_H
#define CRDTPP_MULTI_IDENTIFIER_H

#include <iostream>


class QIdentifier;

class Identifier {

    int digit;
    std::string siteId;

public:
    Identifier(int digit, std::string siteId);

    int getDigit() const;

    void setDigit(int digit);

    std::string getSiteId() const;

    void setSiteId(std::string siteId);

    bool operator==(const Identifier &rhs) const;

    bool operator<(const Identifier &rhs) const;

    QIdentifier toSerializable();
};


#endif //CRDTPP_MULTI_IDENTIFIER_H
