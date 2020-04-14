//
// Created by asant on 28/03/2020.
//

#ifndef CRDTPP_MULTI_IDENTIFIER_H
#define CRDTPP_MULTI_IDENTIFIER_H


class Identifier {

    int digit;
    int siteId;

public:
    Identifier(int digit, int siteId);

    int getDigit() const;

    void setDigit(int digit);

    int getSiteId() const;

    void setSiteId(int siteId);

    bool operator==(const Identifier &rhs) const;

    bool operator<(const Identifier &rhs) const;
};


#endif //CRDTPP_MULTI_IDENTIFIER_H
