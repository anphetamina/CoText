//
// Created by Emanuele Munafò on 13/04/2020.
//
#pragma  once
#include <cstdint>


class User {
public:
    User();
    ~User();

public:
    uint8_t signature = 0x00;
};

