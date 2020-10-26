#include <algorithm>
#include <iostream>
#include "SharedEditor.h"
#include "Shuffler.h"
#include <QDebug>

#ifdef _MSC_VER
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif

SharedEditor::SharedEditor(int siteId)
        : siteId(siteId), counter(0), base(32), boundary(10), idCounter(0) {
    symbols.emplace_back();
}

bool SharedEditor::retrieveStrategy(int level) {
    if (level < 0) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": level is negative");
    }
    if (level < strategies.size() && level != 0) {
        return strategies.at(level);
    }
    bool strategy;
    std::uniform_int_distribution<int> distribution(1, 10);
    int n = distribution(Shuffler::getInstance()->getGenerator());
    strategy = n % 2 == 0;
    this->strategies.insert(std::pair<int,bool>(level, strategy));
    return strategy;
}

/**
 *
 * @param min
 * @param max
 * @param strategy
 * @return random number in ]min, max[
 */
int SharedEditor::generateIdBetween(int min, int max, bool strategy) {

    if (min < 0 && max < 0) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": min and max are negative");
    }
    if (min < 0) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": min is negative");
    }
    if (max < 0) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": max is negative");
    }
    if (max < min) {
        throw std::range_error(std::string{} + __PRETTY_FUNCTION__ + ": min is greater than max");
    }
    if (min == max) {
        throw std::range_error(std::string{} + __PRETTY_FUNCTION__ + ": min is equal to max");
    }

    if ((max - min) == 2) {
        /**
         * min+1 is the only number available
         */
        return min + 1;
    } else if ((max - min) < boundary) {
        /**
         * increase the lower bound and decreased the upper bound
         */
        min = min + 1;
        max = max - 1;
    } else {
        if (!strategy) { // boundary-
            min = max - boundary + 1;
            max = max - 1;
        } else { // boundary+
            max = min + boundary - 1;
            min = min + 1;
        }
    }
    std::uniform_int_distribution<int> distribution(min, max);
    int id = distribution(Shuffler::getInstance()->getGenerator());
    return id;
}

/**
 *
 * @param line
 * @param index
 * @return the previous fractional position from line and index
 */
std::vector<Identifier> SharedEditor::findPosBefore(int line, int index) {

    if (index == 0 && line == 0) {
        /**
         * get the lowest fractional position
         */
        return std::vector<Identifier>{Identifier(0, siteId)};
    } else if (index == 0 && line != 0) {
        /**
         * get the lowest fractional position in the previous line
         */
        line = line - 1;
        index = symbols[line].size();
    }

    return symbols[line].at(index-1).getPosition();
}

/**
 *
 * @param line
 * @param index
 * @return the next fractional position from line and index
 */
std::vector<Identifier> SharedEditor::findPosAfter(int line, int index) {

    int nLines = symbols.size();

    if (line == nLines - 1 && index == symbols[nLines - 1].size()) {
        /**
         * get the highest fractional position
         */
        return {Identifier(base, siteId)};
    } else if (line < nLines - 1 && index == symbols[line].size()) {
        /**
         * get the lowest fractional position in the next line
         */
        line++;
        index = 0;
    }

    return symbols[line].at(index).getPosition();
}

/**
 *
 * @param pos1
 * @param pos2
 * @param newPos
 * @param level
 * @return fractional pos between pos1 and pos2
 */
std::vector<Identifier> SharedEditor::generatePosBetween(std::vector<Identifier> pos1, std::vector<Identifier> pos2, std::vector<Identifier> newPos, int level) {
    if (pos1.empty()) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": pos1 is empty");
    }
    if (level < 0) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": level is negative");
    }

    int id1 = 0;
    int id2 = static_cast<int>(std::pow(2, level)*base);
    int siteId1 = siteId;
    int siteId2 = siteId;
    /**
     * get the current lowest and highest fractional position at the current level
     */
    if (pos1.size() > level) {
        id1 = pos1.at(level).getDigit();
        siteId1 = pos1.at(level).getSiteId();
    }
    if (!pos2.empty() && pos2.size() > level) {
        id2 = pos2.at(level).getDigit();
        siteId2 = pos2.at(level).getSiteId();
    }

    bool boundaryStrategy = retrieveStrategy(level);

    try {
        if ((id2 - id1) > 1) {
            int newId = 0;
            newId = generateIdBetween(id1, id2, boundaryStrategy);
            newPos.emplace_back(newId, siteId);
            return newPos;
        } else if ((id2 - id1) == 1) {
            /**
             * there is no space between two ids and it needs to create a new level
             */
            newPos.emplace_back(id1, siteId1);
            return this->generatePosBetween(pos1, {}, newPos, level+1);
        } else if (id1 == id2) {
            /**
             * always pick the lowest siteid to guarantee the order
             */
            if (siteId1 < siteId2) {
                newPos.emplace_back(id1, siteId1);
                return this->generatePosBetween(pos1, {}, newPos, level + 1);
            } else if (siteId1 == siteId2) {
                newPos.emplace_back(id1, siteId1);
                return this->generatePosBetween(pos1, pos2, newPos, level + 1);
            } else {
                newPos.emplace_back(id1, siteId2);
                return this->generatePosBetween(pos1, pos2, newPos, level+1);
            }
        } else {
            throw std::runtime_error(std::string{} + __PRETTY_FUNCTION__ + ":invalid symbols ordering");
        }
    } catch (...) {
        throw;
    }


}

/**
 * insert symbol at line in index
 * @param line
 * @param index
 * @param symbol
 */
void SharedEditor::insertSymbol(int line, int index, QSymbol symbol) {

    if (symbol.isNewLine()) {

        std::vector<QSymbol> lineAfter;

        if (index == symbols[line].size()) {
            /**
             * new line symbol at the end of line
             */
            lineAfter = {};
        } else {
            /**
             * otherwise copy in the line after the content that comes after the new line
             */
            lineAfter.assign(symbols[line].begin() + index, symbols[line].end());
        }

        if (lineAfter.empty()) {
            symbols[line].push_back(symbol);
            symbols.emplace_back();
        } else {
            /**
             * copies the content of the line before in line
             * and insert the line after content in the next one
             */
            std::vector<QSymbol> lineBefore(symbols[line].begin(), symbols[line].begin() + index);
            lineBefore.push_back(symbol);
            symbols[line] = lineBefore;
            symbols.insert(symbols.begin() + line + 1, lineAfter);
        }
    } else {
        symbols[line].insert(symbols[line].begin() + index, symbol);
    }

    counter++;
    idCounter++;
}

/**
 *
 * @param line
 * @param index
 * @param value
 * @param format
 * insert value at index in symbols
 * a tail insert must be done before a \n
 * so if we have a CRLF char at the end of the line
 * the insert after the \n is replaced with an head insert in the next line
 * e.g.
 * 0) luca\n
 * 1)
 *
 * the insert in (0,5) is replaced with the insert in (1,0)
 */
QSymbol SharedEditor::localInsert(int line, int index, QChar value, QTextCharFormat format) {

    if (line < 0) {
        throw std::out_of_range(std::string{} + __PRETTY_FUNCTION__ + ": line "+std::to_string(line)+" is negative");
    } else if (line >= symbols.size()) {
        throw std::out_of_range(std::string{} + __PRETTY_FUNCTION__ + ": line "+std::to_string(line)+" >= "+std::to_string(symbols.size()));
    } else if (index < 0) {
        throw std::out_of_range(std::string{} + __PRETTY_FUNCTION__ + ": index "+std::to_string(index)+" is negative");
    } else if (index > symbols[line].size()) {
        throw std::out_of_range(std::string{} + __PRETTY_FUNCTION__ + ": index "+std::to_string(index)+" > "+std::to_string(symbols[line].size()));
    }


    QString sym_id = QString::number(siteId);
    sym_id.append('-');
    sym_id.append(QString::number(idCounter));
    std::vector<Identifier> sym_position;
    std::vector<Identifier> pos1;
    std::vector<Identifier> pos2;
    QSymbol sym(value, sym_id, {}, format);

    if (!symbols[line].empty()) {
        /**
         * adjust the coordinates if the new symbol is inserted after a line feed at the end of line
         */
        if (index == symbols[line].size() && symbols[line].back().isNewLine()) {
            line++;
            index = 0;
        }
    }

    try {
        pos1 = findPosBefore(line, index);
        pos2 = findPosAfter(line, index);
        sym_position = generatePosBetween(pos1, pos2, sym_position, 0);
        sym.setPosition(sym_position);
        insertSymbol(line, index, sym);

    } catch (std::exception& e) {
        std::cerr << "line " << line << ", index " << index << ", value " << QString(sym.getC()).toStdString() << ", siteId " << siteId << std::endl;
        std::cerr << e.what() << std::endl;
    }
    return sym;
}

/**
 *
 * @param startLine
 * @param startIndex
 * @param endLine
 * @param endIndex
 * @return erased symbols in [startIndex, endIndex]
 */
std::vector<QSymbol> SharedEditor::eraseSingleLine(int startLine, int startIndex, int endLine, int endIndex) {
    endIndex++;

    std::vector<QSymbol> erasedSymbols(symbols[startLine].begin() + startIndex, symbols[startLine].begin() + endIndex);
    symbols[startLine].erase(symbols[startLine].begin() + startIndex, symbols[startLine].begin() + endIndex);
    counter -= (endIndex - startIndex);

    return erasedSymbols;
}

/**
 *
 * @param startLine
 * @param startIndex
 * @param endLine
 * @param endIndex
 * @return erased symbols in [startIndex, endIndex] from [startLine, endLine]
 */
std::vector<QSymbol> SharedEditor::eraseMultipleLines(int startLine, int startIndex, int endLine, int endIndex) {

    std::vector<QSymbol> erasedSymbols(symbols[startLine].begin() + startIndex, symbols[startLine].end());
    for (int i = startLine+1; i < endLine; i++) {
        erasedSymbols.insert(erasedSymbols.end(), symbols[i].begin(), symbols[i].end());
    }
    erasedSymbols.insert(erasedSymbols.end(), symbols[endLine].begin(), symbols[endLine].begin() + endIndex+1);
    symbols[startLine].erase(symbols[startLine].begin() + startIndex, symbols[startLine].end());
    symbols[endLine].erase(symbols[endLine].begin(), symbols[endLine].begin() + endIndex + 1);

    /**
     * erase all the lines between startLine and endLine
     */
    if (endLine-startLine != 1) {
        symbols.erase(symbols.begin() + startLine + 1, symbols.begin() + endLine);
    }

    counter -= erasedSymbols.size();

    return erasedSymbols;
}

/**
 * erase symbols in [startIndex, endIndex] from [startLine, endLine]
 * @param startLine
 * @param startIndex
 * @param endLine
 * @param endIndex
 */
std::vector<QSymbol> SharedEditor::localErase(int startLine, int startIndex, int endLine, int endIndex) {

    if (startLine < 0) {
        throw std::out_of_range(std::string{} + __PRETTY_FUNCTION__ + ": startLine "+std::to_string(startLine)+" is negative");
    } else if (endLine >= symbols.size()) {
        throw std::out_of_range(std::string{} + __PRETTY_FUNCTION__ + ": endLine "+std::to_string(endLine)+" > num of lines "+std::to_string(symbols.size()));
    } else if (startIndex < 0) {
        throw std::out_of_range(std::string{} + __PRETTY_FUNCTION__ + ": startIndex "+std::to_string(startIndex)+" is negative");
    } else if (endIndex >= symbols[endLine].size()) {
        throw std::out_of_range(std::string{} + __PRETTY_FUNCTION__ + ": endIndex "+std::to_string(endIndex)+" > num of symbols "+std::to_string(symbols[endLine].size())+" line "+std::to_string(endLine));
    } else if (startLine > endLine) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": startLine "+std::to_string(startLine)+" > endLine "+std::to_string(endLine));
    } else if (startLine == endLine && startIndex > endIndex) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": startIndex "+std::to_string(startLine)+" > endIndex "+std::to_string(endIndex));
    }

    std::vector<QSymbol> erasedSymbols;
    bool mergeLines = false;
    if (symbols[0].empty() || symbols.size() <= startLine) {
        return {};
    } else if (startLine != endLine) {
        erasedSymbols = eraseMultipleLines(startLine, startIndex, endLine, endIndex);
        /**
         * check if the latest erased symbol is a new line
         * if true the next one will be erased as well
         * in order to avoid a pending empty line
         */
        if (erasedSymbols.back().isNewLine()) {
            symbols.erase(symbols.begin() + startLine + 1);
        }
        mergeLines = true;
    } else {
        erasedSymbols = eraseSingleLine(startLine, startIndex, endLine, endIndex);
        if (erasedSymbols.back().isNewLine()) {
            mergeLines = true;
        }
    }

    /**
     * merge only if it needs to and there is at least one symbol
     */
    if (mergeLines && !(symbols[0].empty() && symbols.size() == 1)) {
        symbols[startLine].insert(symbols[startLine].end(), symbols[startLine+1].begin(), symbols[startLine+1].end());
        symbols.erase(symbols.begin() + startLine + 1);
    }

    return erasedSymbols;

}


/**
 *
 * @param symbol
 * @return the pair (line,index) and insert symbol right before the first one with the higher fractional position
 */
std::pair<int, int> SharedEditor::remoteInsert(const QSymbol &symbol) {

    if (!symbol.isValid()) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": symbol is invalid");
    }

    if (symbols.front().empty()) {
        insertSymbol(0, 0, symbol);
        return std::make_pair(0, 0);
    }

    std::vector<std::vector<QSymbol>>::iterator line_it;
    std::vector<std::vector<QSymbol>>::iterator last;
    if (symbols.back().empty()) {
        last = symbols.end()-1;
    } else {
        last = symbols.end();
    }
    line_it = std::lower_bound(symbols.begin(), last, symbol, [](const std::vector<QSymbol> & it, const QSymbol& symbol){
        return it[0] < symbol;
    });

    if (!(line_it == last) && !(line_it == symbols.begin() || line_it->front().getPosition() == symbol.getPosition())) {
        line_it--;
    } else if (line_it == last && !(line_it == symbols.begin())) {
        line_it--;
    }
    int line = line_it - symbols.begin();

    if (*line_it->begin() == symbol) {
        if (symbol.isNewLine()) {
            qDebug() << "remoteInsert symbol 'CRLF' ("+symbol.getId()+") already exists";
        } else {
            qDebug() << "remoteInsert symbol '"+ QString(symbol.getC()) +"' ("+symbol.getId()+") already exists";
        }

    } else {
        std::vector<QSymbol>::iterator index_it;
        index_it = std::lower_bound(line_it->begin(), line_it->end(), symbol);
        int index = index_it - line_it->begin();

        if (!(index_it == line_it->end()) && !(index_it == line_it->begin() || index_it->getPosition() == symbol.getPosition())) {
            index_it--;
        } else if (index_it == line_it->end() && !(index_it == line_it->begin())) {
            index_it--;
        }

        if (*index_it == symbol) {
            if (symbol.isNewLine()) {
                qDebug() << "remoteInsert symbol 'CRLF' ("+symbol.getId()+") already exists";
            } else {
                qDebug() << "remoteInsert symbol '"+ QString(symbol.getC()) +"' ("+symbol.getId()+") already exists";
            }
        } else {
            if (index_it->isNewLine() && index_it->getPosition() < symbol.getPosition()) {
                line++;
                index = 0;
            }

            insertSymbol(line, index, symbol);

            idCounter--;

            return std::make_pair(line, index);
        }
    }

    return std::make_pair(-1, -1);

}

/**
 *
 * @param symbol
 * @return the pair (line,index) of the removed symbol
 */
std::pair<int, int> SharedEditor::remoteErase(const QSymbol &symbol) {

    if (!symbol.isValid()) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": symbol is invalid");
    }

    if (!symbols.front().empty()) {
        bool mergeLines = false;
        std::vector<std::vector<QSymbol>>::iterator line_it;
        std::vector<std::vector<QSymbol>>::iterator last;
        if (symbols.back().empty()) {
            last = symbols.end()-1;
        } else {
            last = symbols.end();
        }
        line_it = std::lower_bound(symbols.begin(), last, symbol, [](const std::vector<QSymbol> & it, const QSymbol& symbol){
            return it[0] < symbol;
        });

        /*
         * A = line_it = symbols.begin() (first symbol)
         * B = line_it->front().getPosition() == symbol.getPosition()
         * C = line_it == last
         *
         * C A B
         * 1 0 / -> 1
         * 1 1 / -> 0
         *
         * 0 0 0 -> 1
         * 0 0 1 -> 0
         * 0 1 0 -> 0
         * 0 1 1 -> 0
         */

        if (!(line_it == last) && !(line_it == symbols.begin() || line_it->front().getPosition() == symbol.getPosition())) {
            line_it--;
        } else if (line_it == last && !(line_it == symbols.begin())) {
            line_it--;
        }
        int line = line_it - symbols.begin();

        std::vector<QSymbol>::iterator index_it;
        index_it = std::find(line_it->begin(), line_it->end(), symbol);

        if (index_it == line_it->end()) {
            if (symbol.isNewLine()) {
                qDebug() << "remoteErase symbol 'CRLF' ("+symbol.getId()+") not found";
            } else {
                qDebug() << "remoteErase symbol '"+ QString(symbol.getC()) +"' ("+symbol.getId()+") not found";
            }
        } else {
            int index = index_it - line_it->begin();

            if (index_it->isNewLine()) {
                mergeLines = true;
            }
            eraseSingleLine(line, index, line, index);

            if (mergeLines) {
                line_it->insert(line_it->end(), (line_it + 1)->begin(), (line_it + 1)->end());
                symbols.erase(line_it + 1);
            }

            return std::make_pair(line, index);
        }
    }

    return std::make_pair(-1, -1);
}

std::pair<int, int> SharedEditor::getPos(const QSymbol &symbol) {

    if (!symbol.isValid()) {
        throw std::invalid_argument(std::string{} + __PRETTY_FUNCTION__ + ": symbol is invalid");
    }

    std::pair<int, int> pos = std::make_pair(-1, -1);

    if (!symbols.front().empty()) {
        std::vector<std::vector<QSymbol>>::iterator line_it;
        std::vector<std::vector<QSymbol>>::iterator last;
        if (symbols.back().empty()) {
            last = symbols.end()-1;
        } else {
            last = symbols.end();
        }
        line_it = std::lower_bound(symbols.begin(), last, symbol, [](const std::vector<QSymbol> & it, const QSymbol& symbol) {
            return it[0] < symbol;
        });


        if (!(line_it == last) && !(line_it == symbols.begin() || line_it->front().getPosition() == symbol.getPosition())) {
            line_it--;
        } else if (line_it == last && !(line_it == symbols.begin())) {
            line_it--;
        }
        int line = line_it - symbols.begin();
        pos.first = line;

        std::vector<QSymbol>::iterator index_it;
        index_it = std::find(line_it->begin(), line_it->end(), symbol);

        if (index_it == line_it->end()) {
            if (symbol.isNewLine()) {
                qDebug() << "symbol 'CRLF' ("+symbol.getId()+") not found";
            } else {
                qDebug() << "symbol '"+ QString(symbol.getC()) +"' ("+symbol.getId()+") not found";
            }
        } else {
            int index = index_it - line_it->begin();
            pos.second = index;
        }
    }

    return pos;
}

int SharedEditor::getSiteId() const {
    return siteId;
}

const std::vector<std::vector<QSymbol>>& SharedEditor::getSymbols() const {
    return symbols;
}

const QSymbol &SharedEditor::getSymbol(int row, int col) const {

    if (row >= symbols.size()) {
        throw std::out_of_range("row is greater than symbols size");
    }

    const std::vector<QSymbol> &line = symbols[row];

    if (col >= line.size()) {
        throw std::out_of_range("col " +std::to_string(col)+ " is greater than line " +std::to_string(row)+ " size");
    }

    return line[col];
}

void SharedEditor::clear() {
    symbols.clear();
    symbols.emplace_back();
    strategies.clear();
    counter = 0;
    idCounter = 0;
}

void SharedEditor::setSymbols(std::vector<std::vector<QSymbol>> symbols) {
    this->symbols.clear();
    strategies.clear();
    this->symbols = symbols;
    counter = std::accumulate(symbols.begin(), symbols.end(), 0, [](int acc, const std::vector<QSymbol>& a){ return acc+a.size(); });
    idCounter = 0;
}

void SharedEditor::setSiteId(int siteId){
    this->siteId = siteId;
}

std::vector<QSymbol> SharedEditor::getBlock(int startLine, int startIndex, int endLine, int endIndex) {
    std::vector<QSymbol> block;

    if (symbols[startLine].empty()) {
        return {};
    } else if (startLine == endLine) {
        for (int i = startIndex; i <= endIndex; i++) {
            block.push_back(symbols[startLine][i]);
        }
    } else {
        for (int i = startIndex; i < symbols[startLine].size(); i++) {
            block.push_back(symbols[startLine][i]);
        }

        for (int i = startLine + 1; i <= endLine-1; i++) {
            for (int j = 0; j < symbols[i].size(); j++) {
                block.push_back(symbols[i][j]);
            }
        }

        for (int i = 0; i <= endIndex; i++) {
            block.push_back(symbols[endLine][i]);
        }
    }

    return block;
}

int SharedEditor::getConnectedUsers() {
    return this->connectedUsers;
}

void SharedEditor::increaseConnectedUsers() {
    this->connectedUsers++;
}

void SharedEditor::decreaseConnectedUsers() {
    this->connectedUsers++;
}