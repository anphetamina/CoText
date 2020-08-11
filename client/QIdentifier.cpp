//
// Created by Antonio Santoro on 09/08/2020.
//

#include "QIdentifier.h"

QIdentifier::QIdentifier(qint32 digit, qint32 siteId) : digit(digit), siteId(siteId) {}
//QIdentifier::QIdentifier(const qint32 digit, const qint32 siteId) : digit(digit), siteId(siteId) {}

QIdentifier::QIdentifier() {}

qint32 QIdentifier::getDigit() {
    return digit;
}

qint32 QIdentifier::getSiteId() {
    return siteId;
}

void QIdentifier::setDigit(qint32 digit) {
    QIdentifier::digit = digit;
}

void QIdentifier::setSiteId(qint32 siteId) {
    QIdentifier::siteId = siteId;
}

Identifier QIdentifier::toOriginal() {
    return Identifier(digit, siteId);
}

QDataStream& operator<<(QDataStream& s, const QIdentifier &qi) {
    s << qi.digit << qi.siteId ;
    return s;
}
QDataStream & operator>>(QDataStream& s, QIdentifier& qi) {
    s >> qi.digit >> qi.siteId ;
    return s;
}

