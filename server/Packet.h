//
// Created by Emanuele Munafò on 13/04/2020.
//

#ifndef TOGHEDITSERVER_PACKET_H
#define TOGHEDITSERVER_PACKET_H


#include <cstdint>
#include <vector>

class Packet
{
public:
    Packet();
    ~Packet();

    /*
    Serializes the packet to send over network .
    Fields are written to buffer already in network byte order (big-endian).
    */
    void serialize(std::vector<uint8_t>& buf);

public:
    uint8_t header = 0xAF; // Identify the beginning of toghedit packet`
    uint8_t flags = 0x00; // Custom bitmask flags.
    uint16_t type = 0x0000; // Identifies the packet's type - what is it used for (composed by 2 bytes)
    uint32_t size = 0x00000000; // Size of `data`.
    std::vector<uint8_t> data; // Custom data.
    uint8_t signature = 0x00; // Small signature (should be much and much bigger for a good security)
    // signature is used for all packages *sent by the client* except the login one
    //uint8_t signature = 0x00; // Eventually add CRC or some error detection or correction system (not needed here, tcp is used)
};


#endif //TOGHEDITSERVER_PACKET_H
