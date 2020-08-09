//
// Created by Antonio Santoro on 09/08/2020.
//

#include "QIdentifier.h"

QIdentifier::QIdentifier(qint32 digit, qint32 siteId) : digit(digit), siteId(siteId) {}

qint32 QIdentifier::getDigit() const {
    return digit;
}

qint32 QIdentifier::getSiteId() const {
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
