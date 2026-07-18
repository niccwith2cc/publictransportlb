#ifndef TRAM_H
#define TRAM_H

#include <cstdint>
#include "Transport.h"

class Tram: public Transport{
public:
    Tram(std::uint8_t carts, std::uint16_t cartCapacity);
    
    ~Tram() override;

};

#endif