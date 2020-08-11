//
// Created by Antonio Santoro on 09/08/2020.
//

#include "QIdentifier.h"

QIdentifier::QIdentifier(qint32 digit, QString siteId) : digit(digit), siteId(siteId) {}

qint32 QIdentifier::getDigit() const {
    return digit;
}

QString QIdentifier::getSiteId() const {
    return siteId;
}

void QIdentifier::setDigit(qint32 digit) {
    QIdentifier::digit = digit;
}

void QIdentifier::setSiteId(QString siteId) {
    QIdentifier::siteId = siteId;
}

Identifier QIdentifier::toOriginal() {
    return Identifier(digit, siteId.toStdString());
}
