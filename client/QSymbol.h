//
// Created by Antonio Santoro on 09/08/2020.
//

#ifndef COTEXT_CLIENT_QSYMBOL_H
#define COTEXT_CLIENT_QSYMBOL_H


#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QVector>
#include "QIdentifier.h"
#include "Symbol.h"

class QSymbol {

    QChar c;
    QString id;
    QVector<QIdentifier> position;

public:

    QSymbol(const QChar &c, const QString &id, const QVector<QIdentifier> &position);

    void setC(const QChar &c);

    void setId(const QString &id);

    void setPosition(const QVector<QIdentifier> &position);

    const QChar &getC() const;

    const QString &getId() const;

    const QVector<QIdentifier> &getPosition() const;

    Symbol toOriginal();

};


#endif //COTEXT_CLIENT_QSYMBOL_H
