//
// Created by Emanuele Munafò on 03/10/2020.
//

#include <iostream>
#include <iomanip>
#include "Benchmark.h"

Benchmark::Benchmark():name(""){};
Benchmark::Benchmark(std::string name):name(name){};

void Benchmark::startTimer() {
    start = clock();
}
void Benchmark::stopTimer() {
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    std::cout << "Time taken by" << name << " is : " << std::fixed
         << time_taken << std::setprecision(5);
    std::cout << " sec " << std::endl;
}
