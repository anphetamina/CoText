#define INSERT 1
#define DELETE -1

#include <algorithm>
#include <iostream>
#include <sstream>
#include "SharedEditor.h"
#include "NetworkServer.h"
#include "Shuffler.h"

SharedEditor::SharedEditor(NetworkServer &server)
        : server(server), counter(0), base(32), boundary(10), idCounter(0) {
    symbols.emplace_back();
    siteId = server.connect(this);
}

bool SharedEditor::retrieveStrategy(int level) {
    if (level < 0) {
        throw std::invalid_argument("level is negative");
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
        throw std::invalid_argument("min and max is negative");
    }
    if (min < 0) {
        throw std::invalid_argument("min is negative");
    }
    if (max < 0) {
        throw std::invalid_argument("max is negative");
    }
    if (max < min) {
        throw std::range_error("min is greater than max");
    }
    if (min == max) {
        throw std::range_error("min is equal to max");
    }

    if ((max - min) == 2) {
        return min + 1;
    } else if ((max - min) < boundary) {
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
 * @param index
 * @return the position of the next previous symbol from pos
 */
std::vector<Identifier> SharedEditor::findPosBefore(int line, int index) {

    if (index == 0 && line == 0) {
        return std::vector<Identifier>{Identifier(0, siteId)};
    } else if (index == 0 && line != 0) {
        line = line - 1;
        index = symbols[line].size();
    }

    return symbols[line].at(index-1).getPosition();
}

/**
 *
 * @param index
 * @return the position of the next following symbol from pos
 */
std::vector<Identifier> SharedEditor::findPosAfter(int line, int index) {

    int nLines = symbols.size();

    if (line == nLines - 1 && index == symbols[nLines - 1].size()) {
        return {Identifier(base, siteId)};
    } else if (line < nLines - 1 && index == symbols[line].size()) {
        line++;
        index = 0;
    }/* else if (line > nLines - 1 && index == 0) {
        return {Identifier(base, siteId)};
    }*/

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
        throw std::invalid_argument("pos1 is empty");
    }
    if (level < 0) {
        throw std::invalid_argument("level is negative");
    }

    int id1 = 0;
    int id2 = static_cast<int>(std::pow(2, level)*base);
    int siteId1 = siteId;
    int siteId2 = siteId;
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
            newPos.emplace_back(id1, siteId1);
            return this->generatePosBetween(pos1, {}, newPos, level+1);
        } else if (id1 == id2) {
            if (siteId1 < siteId2) {
                newPos.emplace_back(id1, siteId1);
                return this->generatePosBetween(pos1, {}, newPos, level + 1);
            } else if (siteId1 == siteId2) {
                newPos.emplace_back(id1, siteId1);
                return this->generatePosBetween(pos1, pos2, newPos, level + 1);
            } else {
                /*std::stringstream err;
                err << "pos1 {";
                for (auto identifier: pos1) {
                    err << " (" << identifier.getDigit() << ", " << identifier.getSiteId() << ")";
                }
                err << " }" << std::endl;
                err << "pos2 {";
                for (auto identifier: pos2) {
                    err << " (" << identifier.getDigit() << ", " << identifier.getSiteId() << ")";
                }
                err << " }" << std::endl;
                err << "newPos {";
                for (auto identifier: newPos) {
                    err << " (" << identifier.getDigit() << ", " << identifier.getSiteId() << ")";
                }
                err << " }" << std::endl;
                err << "level " << level << std::endl;*/
                newPos.emplace_back(id1, siteId2);
                return this->generatePosBetween(pos1, pos2, newPos, level+1);
            }
        }
    } catch (...) {
        throw;
    }


}

/**
 *
 * @param pos
 * @param symbol
 * insert symbol at the given pos
 */
void SharedEditor::insertSymbol(int line, int index, Symbol symbol) {

    char value = symbol.getC();

    if (value == '\n') {

        std::vector<Symbol> lineAfter;

        if (index == symbols[line].size()) {
            lineAfter = {};
        } else {
            lineAfter.assign(symbols[line].begin() + index, symbols[line].end());
        }

        if (lineAfter.empty()) {
            symbols[line].push_back(symbol);
            symbols.emplace_back();
        } else {
            std::vector<Symbol> lineBefore(symbols[line].begin(), symbols[line].begin() + index);
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
 * @param index
 * @param value
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
void SharedEditor::localInsert(int line, int index, char value) {

    if (line < 0) {
        throw std::out_of_range("line "+std::to_string(line)+" is negative");
    } else if (line >= symbols.size()) {
        throw std::out_of_range("line "+std::to_string(line)+" >= "+std::to_string(symbols.size()));
    } else if (index < 0) {
        throw std::out_of_range("index "+std::to_string(index)+" is negative");
    } else if (index > symbols[line].size()) {
        throw std::out_of_range("index "+std::to_string(index)+" > "+std::to_string(symbols[line].size()));
    }


    std::string sym_id = std::to_string(siteId);
    sym_id.append("-");
    sym_id.append(std::to_string(idCounter));
    std::vector<Identifier> sym_position;
    std::vector<Identifier> pos1;
    std::vector<Identifier> pos2;
    Symbol sym(value, sym_id, {});

    if (!symbols[line].empty()) {
        if (index >= symbols[line].size() && symbols[line].back().getC() == '\n') {
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
        std::cerr << "line " << line << ", index " << index << ", value " << value << ", siteId " << siteId << std::endl;
        std::cerr << e.what() << std::endl;
        return;
    }

    Message m(INSERT, sym, siteId);
    server.send(m);
}

/**
 *
 * @param startPos
 * @param endPos
 * @return symbols erased from [startPos, endPos] in a single line
 */
std::vector<Symbol> SharedEditor::eraseSingleLine(int startLine, int startIndex, int endLine, int endIndex) {
    endIndex++;

    std::vector<Symbol> erasedSymbols(symbols[startLine].begin() + startIndex, symbols[startLine].begin() + endIndex);
    symbols[startLine].erase(symbols[startLine].begin() + startIndex, symbols[startLine].begin() + endIndex);
    counter -= (endIndex - startIndex);

    return erasedSymbols;
}

/**
 *
 * @param startPos
 * @param endPos
 * @return symbols erased from [startPos, endPos] between multiple lines
 */
std::vector<Symbol> SharedEditor::eraseMultipleLines(int startLine, int startIndex, int endLine, int endIndex) {

    std::vector<Symbol> erasedSymbols(symbols[startLine].begin() + startIndex, symbols[startLine].end());
    for (int i = startLine+1; i < endLine; i++) {
        erasedSymbols.insert(erasedSymbols.end(), symbols[i].begin(), symbols[i].end());
    }
    erasedSymbols.insert(erasedSymbols.end(), symbols[endLine].begin(), symbols[endLine].begin() + endIndex+1);
    symbols[startLine].erase(symbols[startLine].begin() + startIndex, symbols[startLine].end());
    symbols[endLine].erase(symbols[endLine].begin(), symbols[endLine].begin() + endIndex + 1);
    if (endLine-startLine != 1) {
        symbols.erase(symbols.begin() + startLine + 1, symbols.begin() + endLine);
    }
    counter -= erasedSymbols.size();

    return erasedSymbols;
}

/**
 *
 * @param index
 * remove symbols from [startPos, endPos]
 */
void SharedEditor::localErase(int startLine, int startIndex, int endLine, int endIndex) {

    if (startLine < 0) {
        throw std::out_of_range("startLine "+std::to_string(startLine)+" is negative");
    } else if (endLine > symbols.size()) {
        throw std::out_of_range("endLine "+std::to_string(endLine)+" > num of lines "+std::to_string(symbols.size()));
    } else if (startIndex < 0) {
        throw std::out_of_range("startIndex "+std::to_string(startIndex)+" is negative");
    } else if (endIndex > symbols[endLine].size()) {
        throw std::out_of_range("endIndex "+std::to_string(endIndex)+" > num of symbols "+std::to_string(symbols[endLine].size())+" line "+std::to_string(endLine));
    } else if (startLine > endLine) {
        throw std::invalid_argument("startLine "+std::to_string(startLine)+" > endLine "+std::to_string(endLine));
    } else if (startLine == endLine && startIndex > endIndex) {
        throw std::invalid_argument("startIndex "+std::to_string(startLine)+" > endIndex "+std::to_string(endIndex));
    }

    std::vector<Symbol> erasedSymbols;
    bool mergeLines = false;
    if (symbols[0].empty() || symbols.size() <= startLine) {
        return;
    } else if (startLine != endLine) {
        erasedSymbols = eraseMultipleLines(startLine, startIndex, endLine, endIndex);
        if (symbols[startLine + 1].empty()) {
            symbols.erase(symbols.begin() + startLine + 1);
        }
        mergeLines = true;
    } else {
        erasedSymbols = eraseSingleLine(startLine, startIndex, endLine, endIndex);
        if (erasedSymbols.back().getC() == '\n') {
            if (symbols[startLine + 1].empty()) {
                symbols.erase(symbols.begin() + startLine + 1);
            } else {
                mergeLines = true;
            }
        }
    }

    if (mergeLines && !(symbols[0].empty() && symbols.size() == 1)) {
        symbols[startLine].insert(symbols[startLine].end(), symbols[startLine+1].begin(), symbols[startLine+1].end());
        symbols.erase(symbols.begin() + startLine+1);
    }

    for (Symbol sym : erasedSymbols) {
        Message m(DELETE, sym, siteId);
        server.send(m);
    }

}


/**
 *
 * @param symbol
 * insert symbol right before the first one with the higher fractional position
 */
void SharedEditor::remoteInsert(Symbol symbol) {

    if (symbols.front().empty()) {
        insertSymbol(0, 0, symbol);
        return;
    }

    std::vector<std::vector<Symbol>>::iterator line_it;
    std::vector<std::vector<Symbol>>::iterator last;
    if (symbols.back().empty()) {
        last = symbols.end()-1;
    } else {
        last = symbols.end();
    }
    line_it = std::lower_bound(symbols.begin(), last, symbol, [](const std::vector<Symbol> & it, const Symbol& symbol){
        return it[0] < symbol;
    });

    if (!(line_it == last) && !(line_it == symbols.begin() || line_it->front().getPosition() == symbol.getPosition())) {
        line_it--;
    } else if (line_it == last && !(line_it == symbols.begin())) {
        line_it--;
    }
    int line = line_it - symbols.begin();

    if (*line_it->begin() == symbol) {
        if (symbol.getC() == '\n') {
            std::cout << "remoteInsert symbol 'CRLF' ("+symbol.getId()+") already exists" << std::endl;
        } else {
            std::cout << "remoteInsert symbol '"+ std::string(1, symbol.getC()) +"' ("+symbol.getId()+") already exists" << std::endl;
        }

    } else {
        std::vector<Symbol>::iterator index_it;
        index_it = std::lower_bound(line_it->begin(), line_it->end(), symbol);
        int index = index_it - line_it->begin();

        if (!(index_it == line_it->end()) && !(index_it == line_it->begin() || index_it->getPosition() == symbol.getPosition())) {
            index_it--;
        } else if (index_it == line_it->end() && !(index_it == line_it->begin())) {
            index_it--;
        }

        if (*index_it == symbol) {
            if (symbol.getC() == '\n') {
                std::cout << "remoteInsert symbol 'CRLF' ("+symbol.getId()+") already exists" << std::endl;
            } else {
                std::cout << "remoteInsert symbol '"+ std::string(1, symbol.getC()) +"' ("+symbol.getId()+") already exists" << std::endl;
            }
        } else {
            if (index_it->getC() == '\n') {
                line++;
                index = 0;
            }

            insertSymbol(line, index, symbol);

            idCounter--;
        }


    }


}

/**
 *
 * @param symbol
 * remove symbol
 */
void SharedEditor::remoteErase(Symbol symbol) {
    if (!symbols.front().empty()) {
        bool mergeLines = false;
        std::vector<std::vector<Symbol>>::iterator line_it;
        std::vector<std::vector<Symbol>>::iterator last;
        if (symbols.back().empty()) {
            last = symbols.end()-1;
        } else {
            last = symbols.end();
        }
        line_it = std::lower_bound(symbols.begin(), last, symbol, [](const std::vector<Symbol> & it, const Symbol& symbol){
            return it[0] < symbol;
        });

        /*
         * A = line_it = symbols.begin()
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

        std::vector<Symbol>::iterator index_it;
        index_it = std::find(line_it->begin(), line_it->end(), symbol);

        if (index_it == line_it->end()) {
            if (symbol.getC() == '\n') {
                std::cout << "remoteInsert symbol 'CRLF' ("+symbol.getId()+") not found" << std::endl;
            } else {
                std::cout << "remoteInsert symbol '"+ std::string(1, symbol.getC()) +"' ("+symbol.getId()+") not found" << std::endl;
            }
        } else {
            int index = index_it - line_it->begin();

            if (index_it->getC() == '\n') {
                mergeLines = true;
            }
            eraseSingleLine(line, index, line, index);

            if (mergeLines) {
                line_it->insert(line_it->end(), (line_it + 1)->begin(), (line_it + 1)->end());
                symbols.erase(line_it + 1);
            }
        }
    }
}

/**
 *
 * @param m
 * if m.type = 1 insert the symbol respecting the fractional position
 * if m.type = -1 remove the symbol given the fractional position
 */
void SharedEditor::process(const Message &m) {
    Symbol symbol = m.getS();
    switch (m.getType()) {
        case INSERT:
            remoteInsert(symbol);
            break;

        case DELETE:
            remoteErase(symbol);
            break;

        default:
            throw std::runtime_error("process: forbidden action");
    }
}

std::string SharedEditor::to_string() {
    std::string output{};
    for (const auto& line : symbols) {
        for (const auto& symbol : line) {
            output.push_back(symbol.getC());
        }
    }
    return output;
}

void SharedEditor::setServer(NetworkServer &server) {
    this->server = server;
}

int SharedEditor::getBase() const {
    return base;
}

void SharedEditor::setCounter(int counter) {
    this->counter = counter;
}

uint64_t SharedEditor::getIdCounter() const {
    return idCounter;
}

void SharedEditor::setIdCounter(uint64_t idCounter) {
    this->idCounter = idCounter;
}

NetworkServer& SharedEditor::getServer() const {
    return server;
}

int SharedEditor::getSiteId() const {
    return siteId;
}

std::vector<std::vector<Symbol>>& SharedEditor::getSymbols() {
    return symbols;
}

int SharedEditor::getCounter() const {
    return counter;
}

void SharedEditor::setSiteId(int siteId) {
    SharedEditor::siteId = siteId;
}

int SharedEditor::getBoundary() const {
    return boundary;
}

void SharedEditor::setBoundary(int boundary) {
    SharedEditor::boundary = boundary;
}
