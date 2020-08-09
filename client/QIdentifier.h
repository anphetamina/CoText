//
// Created by Antonio Santoro on 09/08/2020.
//

#ifndef COTEXT_CLIENT_QIDENTIFIER_H
#define COTEXT_CLIENT_QIDENTIFIER_H


#include <QtCore/QtGlobal>
#include "Identifier.h"

class QIdentifier {
    qint32 digit;
    qint32 siteId;

public:
    QIdentifier(qint32 digit, qint32 siteId);

    void setDigit(qint32 digit);

    void setSiteId(qint32 siteId);

    qint32 getDigit() const;

    qint32 getSiteId() const;

    Identifier toOriginal();
};


#endif //COTEXT_CLIENT_QIDENTIFIER_H
