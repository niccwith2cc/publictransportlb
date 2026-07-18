#include "Tram.h"

Tram::Tram(std::uint8_t carts, std::uint16_t cartCapacity)
    : Transport(carts, cartCapacity) {}

Tram::~Tram() = default;