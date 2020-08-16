//
// Created by Antonio Santoro on 09/08/2020.
//

#include "QSymbol.h"

QSymbol::QSymbol(){}
QSymbol::QSymbol(const QChar &c, const QString &id, const QVector<Identifier> &position, QTextCharFormat cf) : c(c), id(id),
                                                                                            position(position), cf(cf) {}
QSymbol::QSymbol( QChar &c,  QString &id,  QVector<Identifier> &position, QTextCharFormat cf) : c(c), id(id),
                                                                                            position(position), cf(cf) {}


 QChar &QSymbol::getC()  {
    return c;
}

 QString &QSymbol::getId() {
    return id;
}

 QVector<Identifier> &QSymbol::getPosition()  {
    return position;
}
QTextCharFormat &QSymbol::getcf() {
    return cf;
}
void QSymbol::setC(const QChar &c) {
    QSymbol::c = c;
}

void QSymbol::setId(const QString &id) {
    QSymbol::id = id;
}

void QSymbol::setPosition(QVector<Identifier> &position) {
    QSymbol::position = position;
}

Symbol QSymbol::toOriginal() const {
    return Symbol(c.toLatin1(), id.toStdString(), std::vector(position.begin(), position.end()));
}

QDataStream & operator << (QDataStream & s, const QSymbol &qs) {
    s << qs.c << qs.id << qs.position << qs.cf;
    return s;
}

QDataStream & operator >> (QDataStream & s, QSymbol& qs) {
    s >> qs.c >> qs.id >> qs.position >> qs.cf ;
    return s;
}