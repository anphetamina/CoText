#include "Symbol.h"

Symbol::Symbol(char c, const std::string id, const std::vector<Identifier> position) : c(c), id(id), position(position) {}

char Symbol::getC() const {
    return c;
}

void Symbol::setC(char c) {
    Symbol::c = c;
}

const std::string &Symbol::getId() const {
    return id;
}

void Symbol::setId(const std::string &id) {
    Symbol::id = id;
}

const std::vector<Identifier> &Symbol::getPosition() const {
    return position;
}

void Symbol::setPosition(const std::vector<Identifier> &position) {
    Symbol::position = position;
}

bool Symbol::operator==(const Symbol &symbol) const {
    return this->id == symbol.getId();
}

bool Symbol::operator<(const Symbol &symbol) const {
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
