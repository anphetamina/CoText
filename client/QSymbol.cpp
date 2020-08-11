//
// Created by Antonio Santoro on 09/08/2020.
//

#include "QSymbol.h"

QSymbol::QSymbol(const QChar &c, const QString &id, const QVector<Identifier> &position) : c(c), id(id),
                                                                                            position(position) {}

const QChar &QSymbol::getC() const {
    return c;
}

const QString &QSymbol::getId() const {
    return id;
}

const QVector<Identifier> &QSymbol::getPosition() const {
    return position;
}

void QSymbol::setC(const QChar &c) {
    QSymbol::c = c;
}

void QSymbol::setId(const QString &id) {
    QSymbol::id = id;
}

void QSymbol::setPosition(const QVector<Identifier> &position) {
    QSymbol::position = position;
}

Symbol QSymbol::toOriginal() {
    return Symbol(c.toLatin1(), id.toStdString(), std::vector(position.begin(), position.end()));
}
