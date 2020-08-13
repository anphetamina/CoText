#ifndef PDS_CPP_SYMBOL_H
#define PDS_CPP_SYMBOL_H


#include <string>
#include <vector>
#include "Identifier.h"


class QSymbol;

class Symbol {
private:
    /**
     * char inserted in the document
     */
    char c;

    /**
     * combination of id counter and site id
     */
    std::string id;

    /**
     * fractional position
     */
    std::vector<Identifier> position;
public:
    Symbol();
    Symbol(char c, std::string id, std::vector<Identifier> position);

    char getC() const;

    void setC(char c);

    const std::string &getId() const;

    void setId(const std::string &id);

    const std::vector<Identifier> &getPosition() const;

    void setPosition(const std::vector<Identifier> &position);

    bool operator==(const Symbol &symbol) const;
    bool operator<(const Symbol &symbol) const;

    QSymbol toSerializable();


    int getSiteId() const;
};


#endif //PDS_CPP_SYMBOL_H
