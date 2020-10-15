//
// Created by asant on 28/03/2020.
//

#include <climits>
#include "Shuffler.h"

Shuffler::Shuffler() : generator(static_cast<int>(std::chrono::high_resolution_clock::now().time_since_epoch().count())) {}

Shuffler *Shuffler::getInstance() {
    std::call_once(inited, [&]{
        instance = new Shuffler();
    });
    return instance;
}

std::mt19937& Shuffler::getGenerator() {
    return generator;
}

int Shuffler::getRandomInt() {
    std::uniform_int_distribution<int> distribution(0, INT_MAX);
    return distribution(generator);
}

Shuffler::~Shuffler() {
    delete[] instance;
    delete instance;
}
