//
// Created by Antonio Santoro on 09/08/2020.
//

#ifndef COTEXT_CLIENT_QSYMBOL_H
#define COTEXT_CLIENT_QSYMBOL_H


#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QVector>
#include "Symbol.h"

class QSymbol {

    QChar c;
    QString id;
    QVector<Identifier> position;

public:

    QSymbol(const QChar &c, const QString &id, const QVector<Identifier> &position);

    void setC(const QChar &c);

    void setId(const QString &id);

    void setPosition(const QVector<Identifier> &position);

    const QChar &getC() const;

    const QString &getId() const;

    const QVector<Identifier> &getPosition() const;

    Symbol toOriginal();

};


#endif //COTEXT_CLIENT_QSYMBOL_H
