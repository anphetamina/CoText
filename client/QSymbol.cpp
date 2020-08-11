//
// Created by Antonio Santoro on 09/08/2020.
//

#include "QSymbol.h"

QSymbol::QSymbol(){}
QSymbol::QSymbol(const QChar &c, const QString &id, const QVector<Identifier> &position) : c(c), id(id),
                                                                                            position(position) {}
QSymbol::QSymbol( QChar &c,  QString &id,  QVector<Identifier> &position) : c(c), id(id),
                                                                                            position(position) {}


 QChar &QSymbol::getC()  {
    return c;
}

 QString &QSymbol::getId() {
    return id;
}

 QVector<Identifier> &QSymbol::getPosition()  {
    return position;
}

void QSymbol::setC(const QChar &c) {
    QSymbol::c = c;
}

void QSymbol::setId(const QString &id) {
    QSymbol::id = id;
}

void QSymbol::setPosition( QVector<Identifier> &position) {

    QSymbol::position = position;
}

Symbol QSymbol::toOriginal() const {
    std::vector<Identifier> position = {};
    for (auto identifier : this->position) {
        position.push_back(identifier);//.toOriginal() // Using identifier for now
    }
    return Symbol(c.toLatin1(), id.toStdString(), position);

    //return Symbol(c.toLatin1(), id.toStdString(), std::vector(position.begin(), position.end()));
}

QDataStream & operator << (QDataStream & s, const QSymbol &qs) {
    s << qs.c << qs.id << qs.position ;
    return s;
}

QDataStream & operator >> (QDataStream & s, QSymbol& qs) {
    s >> qs.c >> qs.id >> qs.position ;
    return s;
}