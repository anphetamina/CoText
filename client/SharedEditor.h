#ifndef PDS_CPP_SHAREDEDITOR_H
#define PDS_CPP_SHAREDEDITOR_H

#include <vector>
#include <map>
#include <random>
#include "Symbol.h"

class SharedEditor {
private:

    /**
     * assigned by the server
     */
    int siteId;

    std::vector<std::vector<Symbol>> symbols;

    /**
     * current number of symbols in the document
     */
    int counter;

    /**
     * total number of symbols added in the document
     */
    uint64_t idCounter;

    std::map<int, bool> strategies;
    int base;
    int boundary;
public:
    SharedEditor(int siteId = 0);

    int getBoundary() const;

    void setBoundary(int boundary);

    void setCounter(int counter);

    int getSiteId() const;

    void setSiteId(int siteId);

    std::vector<std::vector<Symbol>>& getSymbols();

    void clear();

    int getCounter() const;

    int getBase() const;

    uint64_t getIdCounter() const;

    void setIdCounter(uint64_t idCounter);

    int generateIdBetween(int n1, int n2, bool strategy);
    std::vector<Identifier> generatePosBetween(std::vector<Identifier> pos1, std::vector<Identifier> pos2, std::vector<Identifier> newPos, int level);
    bool retrieveStrategy(int level);
    std::vector<Identifier> findPosBefore(int line, int index);
    std::vector<Identifier> findPosAfter(int line, int index);
    Symbol localInsert(int line, int index, char value);
    void insertSymbol(int line, int index, Symbol symbol);
    std::vector<Symbol> localErase(int startLine, int startIndex, int endLine, int endIndex);
    std::vector<Symbol> eraseSingleLine(int startLine, int startIndex, int endLine, int endIndex);
    std::vector<Symbol> eraseMultipleLines(int startLine, int startIndex, int endLine, int endIndex);

    std::pair<int, int> remoteInsert(const Symbol &symbol);
    std::pair<int, int> remoteErase(const Symbol &symbol);

    std::string to_string();
};


#endif //PDS_CPP_SHAREDEDITOR_H
