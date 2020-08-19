//
// Created by Antonio Santoro on 09/08/2020.
//

#include <QDebug>
#include "QSymbol.h"

QSymbol::QSymbol(){}
QSymbol::QSymbol(const QChar &c, const QString &id, const std::vector<Identifier> &position, QTextCharFormat cf) : c(c), id(id),
                                                                                            position(position), cf(cf) {}
QSymbol::QSymbol(QChar &c, QString &id, std::vector<Identifier> &position, QTextCharFormat cf) : c(c), id(id),
                                                                                            position(position), cf(cf) {}

QDataStream & operator << (QDataStream & s, const QSymbol &qs) {
    s << qs.c << qs.id << QVector<Identifier>(qs.position.begin(), qs.position.end()) << qs.cf;
    return s;
}

QDataStream & operator >> (QDataStream & s, QSymbol& qs) {
    QVector<Identifier> position(qs.position.begin(), qs.position.end());
    s >> qs.c >> qs.id >> position >> qs.cf;
    return s;
}

QChar QSymbol::getC() const {
    return c;
}

QTextCharFormat QSymbol::getCF() const {
    return cf;
}

const QString &QSymbol::getId() const {
    return id;
}

const std::vector<Identifier> &QSymbol::getPosition() const {
    return position;
}

void QSymbol::setPosition(const std::vector<Identifier> &position) {
    this->position = position;
}

bool QSymbol::operator==(const QSymbol &symbol) const {
    return this->id == symbol.getId();
}

bool QSymbol::operator<(const QSymbol &symbol) const {
    int min = 0;
    if (this->position.size() < symbol.getPosition().size()) {
        min = this->getPosition().size();
    } else {
        min = symbol.getPosition().size();
    }
    for (int i = 0; i < min; i++) {
        if (this->position[i] < symbol.getPosition()[i]) {
            return true;
        } else if (symbol.getPosition()[i] < this->position[i]) {
            return false;
        }
    }
    return this->position.size() < symbol.getPosition().size();
}

int QSymbol::getSiteId() const {
    // todo check site id exceptions
    QString delimiter("-");
    QString siteId = id.split(delimiter).front();
    int iSiteId = siteId.toInt();
    return iSiteId;
}
