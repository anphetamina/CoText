//
// Created by Emanuele Munafò on 03/10/2020.
//

#include <iostream>
#include <iomanip>
#include "Benchmark.h"

Benchmark::Benchmark() : name("") {};

Benchmark::Benchmark(std::string name) : name(name) {};

/**
 * Start the timer
 *
 */
void Benchmark::startTimer() {
    start = clock();
}

/**
 * Stop the timer (previously started) and print the time with 5 float digit precision
 *
 */
void Benchmark::stopTimer() {
    end = clock();
    double time_taken = double(end - start) / double(CLOCKS_PER_SEC);
    std::cout << "Time taken by " << name << " is : " << std::fixed
              << time_taken << std::setprecision(5);
    std::cout << " sec " << std::endl;
}

/**
 * Get  the timer value (previously started) as double
 *
 * @return time_partial
 */
double Benchmark::getTimer() {
    clock_t partial = clock();
    double time_partial = double(partial - start) / double(CLOCKS_PER_SEC);
    return time_partial;
}