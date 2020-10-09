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
    QVector<Identifier> qposition(qs.position.begin(), qs.position.end());
    s >> qs.c >> qs.id >> qposition >> qs.cf;
    qs.position = std::vector<Identifier>(qposition.begin(), qposition.end());
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
    if (position.empty()) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": position is empty");
    }
    this->position = position;
}

bool QSymbol::operator==(const QSymbol &symbol) const {
    return this->id == symbol.getId();
}

bool QSymbol::operator<(const QSymbol &symbol) const {

    if (symbol.position.empty()) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": symbol position is empty");
    }

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
    QString delimiter("-");
    QString siteId = id.split(delimiter).front();
    int iSiteId = siteId.toInt();

    if (siteId.isEmpty() || iSiteId == 0) {
        throw std::runtime_error(std::string{} + __PRETTY_FUNCTION__ + ": invalid site id");
    }

    return iSiteId;
}

bool QSymbol::isValid() const {
    return c.isNull() ||
           id.trimmed().isEmpty() ||
           position.empty() ||
           std::any_of(position.begin(), position.end(), [](const Identifier &i) { return !i.isValid(); }) ||
           cf.isEmpty() ||
           !cf.isValid();
}

bool QSymbol::isNewLine() const {
    return c == QChar::LineFeed || c == QChar::ParagraphSeparator || c == QChar::LineSeparator || c=="\u2029";
}

/**
 * Convert a bidimensional qvector of qsymbolsto a  bidimensional std::vector of symbols to
 *
 * @param qsymbols
 * @return symbols
 */
std::vector<std::vector<QSymbol>> toVector(QVector<QVector<QSymbol>> qsymbols){
    std::vector<std::vector<QSymbol>> symbols = {};
    for (auto symbolQArr : qsymbols) {
        std::vector<QSymbol> symbolArr = {};
        for (auto qsymbol : symbolQArr) {  // Iterate over the Symbols
            symbolArr.push_back(qsymbol);
        }
        symbols.push_back(symbolArr);
    }

    return symbols;
}

/**
 * Convert a bidimensional std::vector of symbols to a bidimensional qvector of qsymbols
 *
 * @param symbols
 * @return qsymbols
 */
//
QVector<QVector<QSymbol>> toQVector(std::vector<std::vector<QSymbol>> symbols){
    QVector<QVector<QSymbol>> qsymbols = {};
    for (auto symbolArr : symbols) {
        QVector<QSymbol> qsymbolArr = {};
        for (auto symbol : symbolArr) {  // Iterate over the Symbols
            qsymbolArr.push_back(symbol);
        }
        qsymbols.push_back(qsymbolArr);
    }
    return qsymbols;
}
