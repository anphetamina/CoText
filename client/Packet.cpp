//
// Created by Emanuele Munafò on 13/04/2020.
//
#ifdef _WIN32
  /* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
  #ifndef _WIN32_WINNT
    #define _WIN32_WINNT 0x0501  /* Windows XP. */
  #endif
  #include <winsock2.h>
#endif

#include "Packet.h"

Packet::Packet()
{}

Packet::~Packet()
{}

void Packet::serialize(std::vector<uint8_t>& buf)
{
    buf.push_back(header);
    buf.push_back(flags);
    buf.push_back(htons(type) >> 8);
    buf.push_back(htons(type) >> 0);
    buf.push_back(htonl(size) >> 24);
    buf.push_back(htonl(size) >> 16);
    buf.push_back(htonl(size) >> 8);
    buf.push_back(htonl(size) >> 0);
    for (auto b : data)
        buf.push_back(b);
    buf.push_back(signature);
}