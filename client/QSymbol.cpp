//
// Created by Antonio Santoro on 09/08/2020.
//

#include "QSymbol.h"

QSymbol::QSymbol(const QChar &c, const QString &id, const QVector<QIdentifier> &position) : c(c), id(id),
                                                                                            position(position) {}

const QChar &QSymbol::getC() const {
    return c;
}

const QString &QSymbol::getId() const {
    return id;
}

const QVector<QIdentifier> &QSymbol::getPosition() const {
    return position;
}

void QSymbol::setC(const QChar &c) {
    QSymbol::c = c;
}

void QSymbol::setId(const QString &id) {
    QSymbol::id = id;
}

void QSymbol::setPosition(const QVector<QIdentifier> &position) {
    QSymbol::position = position;
}

Symbol QSymbol::toOriginal() {
    std::vector<Identifier> position = {};
    for (auto identifier : this->position) {
        position.push_back(identifier.toOriginal());
    }
    return Symbol(c.toLatin1(), id.toStdString(), position);
}
