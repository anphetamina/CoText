#ifndef PDS_CPP_SHAREDEDITOR_H
#define PDS_CPP_SHAREDEDITOR_H

#include <vector>
#include <map>
#include <random>
#include "QSymbol.h"

class SharedEditor {
private:

    /**
     * assigned by the server
     */
    int siteId;

    std::vector<std::vector<QSymbol>> symbols;

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

    int connectedUsers = 1;
public:
    SharedEditor(int siteId = 0);

    int getSiteId() const;
    void setSiteId(int siteId);

    const std::vector<std::vector<QSymbol>>& getSymbols() const;

    void setSymbols(std::vector<std::vector<QSymbol>> symbols);

    void clear();

    int generateIdBetween(int n1, int n2, bool strategy);
    std::vector<Identifier> generatePosBetween(std::vector<Identifier> pos1, std::vector<Identifier> pos2, std::vector<Identifier> newPos, int level);
    bool retrieveStrategy(int level);
    std::vector<Identifier> findPosBefore(int line, int index);
    std::vector<Identifier> findPosAfter(int line, int index);
    QSymbol localInsert(int line, int index, QChar value, QTextCharFormat format);
    void insertSymbol(int line, int index, QSymbol symbol);
    std::vector<QSymbol> localErase(int startLine, int startIndex, int endLine, int endIndex);
    std::vector<QSymbol> eraseSingleLine(int startLine, int startIndex, int endLine, int endIndex);
    std::vector<QSymbol> eraseMultipleLines(int startLine, int startIndex, int endLine, int endIndex);

    std::pair<int, int> remoteInsert(const QSymbol &symbol);
    std::pair<int, int> remoteErase(const QSymbol &symbol);

    bool isNewLine(QChar &c);

    std::vector<QSymbol> getBlock(int startLine, int startIndex, int endLine, int endIndex);
    std::pair<int, int> getPos(const QSymbol &symbol);

    void connectedUsersDecrease();

    void connectedUsersIncrease();

    int getConnectedUsers();

    void setConnectedUsers(int connectedUsers);
};


#endif //PDS_CPP_SHAREDEDITOR_H
