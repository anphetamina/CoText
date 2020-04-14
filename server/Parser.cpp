//
// Created by Emanuele Munafò on 13/04/2020.
//

#include "Parser.h"

bool Parser::parse(uint8_t* data, size_t len, size_t& bytesRead, Packet& packet)
{
    bytesRead = 0;
    for (size_t i = 0; i < len; ++i)
    {
        const auto b = data[i];
        bytesRead++;

        switch (_field_step)
        {
            // Header.
            case 0:
                if (b != 0xAF)
                {
                    _field_step = 0;
                    continue;
                }
                packet.header = b;
                _field_step++;
                break;

                // Flags.
            case 1:
                packet.flags = b;
                _field_step++;
                break;

                // Type (2 bytes!).
            case 2:
                packet.type = uint16_t(b) << 8;
                _field_step++;
                break;

            case 3:
                packet.type |= uint16_t(b) << 0;
                packet.type = ntohs(packet.type);
                _field_step++;
                break;

                // Size (4 bytes!).
            case 4:
                packet.size = uint32_t(b) << 24;
                _field_step++;
                break;

            case 5:
                packet.size |= uint32_t(b) << 16;
                _field_step++;
                break;

            case 6:
                packet.size |= uint32_t(b) << 8;
                _field_step++;
                break;

            case 7:
                packet.size |= uint32_t(b) << 0;
                packet.size = ntohl(packet.size);
                _field_step++;

                packet.data.clear();
                if (packet.size > 0)
                {
                    packet.data.reserve(packet.size);
                }
                else
                {
                    _field_step++; // Skip data step.
                }
                break;

                // Data.
            case 8:
                packet.data.push_back(b);

                if (packet.data.size() == packet.size)
                    _field_step++;
                break;

                // Checksum.
            case 9:
                packet.signature = b;
                _field_step = 0;
                return true;

        }
    }
    return false;
}