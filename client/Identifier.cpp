//
// Created by asant on 28/03/2020.
//

#include "Identifier.h"

Identifier::Identifier(int digit, int siteId) : digit(digit), siteId(siteId) {}

int Identifier::getDigit() const {
    return digit;
}

void Identifier::setDigit(int digit) {
    Identifier::digit = digit;
}

int Identifier::getSiteId() const {
    return siteId;
}

void Identifier::setSiteId(int siteId) {
    Identifier::siteId = siteId;
}

bool Identifier::operator==(const Identifier &rhs) const {
    return digit == rhs.digit &&
           siteId == rhs.siteId;
}

bool Identifier::operator<(const Identifier &rhs) const {
    if (digit == rhs.digit) {
        return siteId < rhs.siteId;
    } else {
        return digit < rhs.digit;
    }
}