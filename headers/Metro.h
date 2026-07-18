#ifndef METRO_H
#define METRO_H

#include <cstdint>
#include "Transport.h"

class Metro: public Transport{
public:
    Metro(std::uint8_t carts, std::uint16_t cartCapacity);
    
    ~Metro() override;

};

#endif