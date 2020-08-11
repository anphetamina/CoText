//
// Created by Antonio Santoro on 09/08/2020.
//
#pragma once

#include <cstdint>
#include "Identifier.h"
#include <QtCore/QtGlobal>

class QIdentifier {
    friend QDataStream& operator>>(QDataStream& s, QIdentifier& qi);
    friend QDataStream& operator<<(QDataStream& s, const QIdentifier& qi);

    qint32 digit;
    qint32 siteId;
public:
    QIdentifier();
    QIdentifier(qint32 digit, qint32 siteId);
    //QIdentifier(const qint32 digit, const qint32 siteId);

    void setDigit(qint32 digit);

    void setSiteId(qint32 siteId);

    qint32 getDigit() ;

    qint32 getSiteId() ;

    Identifier toOriginal();
};
