//
// Created by Antonio Santoro on 09/08/2020.
//

#ifndef COTEXT_CLIENT_QIDENTIFIER_H
#define COTEXT_CLIENT_QIDENTIFIER_H


#include <QtCore/QtGlobal>
#include <QtCore/QString>
#include "Identifier.h"

class QIdentifier {
    qint32 digit;
    QString siteId;

public:
    QIdentifier(qint32 digit, QString siteId);

    void setDigit(qint32 digit);

    void setSiteId(QString siteId);

    qint32 getDigit() const;

    QString getSiteId() const;

    Identifier toOriginal();
};


#endif //COTEXT_CLIENT_QIDENTIFIER_H
