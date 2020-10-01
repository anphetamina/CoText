//
// Created by Antonio Santoro on 09/08/2020.
//

#ifndef COTEXT_CLIENT_QSYMBOL_H
#define COTEXT_CLIENT_QSYMBOL_H


#include <QtCore/QChar>
#include <QtCore/QString>
#include <QtCore/QVector>
#include <QTextCharFormat>
#include "Identifier.h"

class QSymbol {

    friend QDataStream& operator>>(QDataStream& s, QSymbol& qs);
    friend QDataStream& operator<<(QDataStream& s, const QSymbol& qs);

    QChar c;

    /**
     * combination of id counter and site id
     */
    QString id;
    std::vector<Identifier> position;
    QTextCharFormat cf;

public:

    QSymbol();
    QSymbol(const QChar &c, const QString &id, const std::vector<Identifier> &position, QTextCharFormat cf);
    QSymbol(QChar &c, QString &id, std::vector<Identifier> &position, QTextCharFormat cf);

    QChar getC() const;
    QTextCharFormat getCF() const;
    const QString &getId() const;
    int getSiteId() const;

    const std::vector<Identifier> &getPosition() const;
    void setPosition(const std::vector<Identifier> &position);

    bool isValid() const;
    bool isNewLine() const;

    bool operator==(const QSymbol &symbol) const;
    bool operator<(const QSymbol &symbol) const;


};
std::vector<std::vector<QSymbol>> toVector(QVector<QVector<QSymbol>> qsymbols);
QVector<QVector<QSymbol>> toQVector(std::vector<std::vector<QSymbol>> symbols);


#endif //COTEXT_CLIENT_QSYMBOL_H
