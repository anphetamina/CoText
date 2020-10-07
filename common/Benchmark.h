//
// Created by Emanuele Munafò on 03/10/2020.
//
#include <ctime>

#ifndef COTEXT_CLIENT_BENCHMARK_H
#define COTEXT_CLIENT_BENCHMARK_H


class Benchmark {

private:
    clock_t start, end;
    std::string name = "";
public:
    Benchmark();

    Benchmark(std::string name);

    void startTimer();

    void stopTimer();

    double getTimer();
};


#endif //COTEXT_CLIENT_BENCHMARK_H
