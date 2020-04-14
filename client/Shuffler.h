//
// Created by asant on 28/03/2020.
//

#ifndef CRDTPP_MULTI_SHUFFLER_H
#define CRDTPP_MULTI_SHUFFLER_H

#include <mutex>
#include <random>

class Shuffler {
private:

    inline static Shuffler* instance;
    inline static std::once_flag inited;
    std::mt19937 generator;

    Shuffler();
public:
    static Shuffler* getInstance();
    std::mt19937& getGenerator();

    ~Shuffler();
};


#endif //CRDTPP_MULTI_SHUFFLER_H
