//
// Created by Antonio Santoro on 09/08/2020.
//
#pragma once

#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QTextCharFormat>
#include "Symbol.h"

class QSymbol {

    friend QDataStream& operator>>(QDataStream& s, QSymbol& qs);
    friend QDataStream& operator<<(QDataStream& s, const QSymbol& qs);

    QChar c;
    QString id;
    QVector<Identifier> position;
    QTextCharFormat cf;
public:
    QSymbol();
    QSymbol(const QChar &c, const QString &id, const QVector<Identifier> &position, QTextCharFormat cf);
    QSymbol( QChar &c,  QString &id,  QVector<Identifier> &position, QTextCharFormat cf);

    //QSymbol(const QChar &c, const QString &id, const QVector<Identifier> &position);

    void setC(const QChar &c);

    void setId(const QString &id);

    void setPosition(QVector<Identifier> &position);

    void setPosition(const QVector<Identifier> &position);

    QChar &getC();

    QString &getId() ;

    QVector<Identifier> &getPosition() ;

    const QVector<Identifier> &getPosition() const;
    QTextCharFormat &getcf() ;

    Symbol toOriginal() const;


};
