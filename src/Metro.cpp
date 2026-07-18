#include "Metro.h"

Metro::Metro(std::uint8_t carts, std::uint16_t cartCapacity)
    : Transport(carts, cartCapacity){}

Metro::~Metro() = default;