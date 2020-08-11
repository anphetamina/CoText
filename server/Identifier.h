//
// Created by asant on 28/03/2020.
//

#ifndef CRDTPP_MULTI_IDENTIFIER_H
#define CRDTPP_MULTI_IDENTIFIER_H


#include <QDataStream>

//class QIdentifier;

class Identifier {
    friend QDataStream& operator>>(QDataStream& s, Identifier& i);
    friend QDataStream& operator<<(QDataStream& s, const Identifier& i);

    int digit;
    int siteId;

public:
    Identifier();
    Identifier(int digit, int siteId);

    int getDigit() const;

    void setDigit(int digit);

    int getSiteId() const;

    void setSiteId(int siteId);

    bool operator==(const Identifier &rhs) const;

    bool operator<(const Identifier &rhs) const;

   //QIdentifier toSerializable();
};


#endif //CRDTPP_MULTI_IDENTIFIER_H
