#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <cstdint>
#include <vector>

class Transport{
private:
    std::uint8_t _numberOfCarts;                   // use to specify the number of carts on this specific transportation type
    std::vector<std::uint16_t> _passengersList;    // find the number of passengers on *each* cart
    std::uint16_t _cartCapacity;                   // how many passengers does one cart hold

public:
    Transport(std::uint8_t carts, std::uint16_t cartCapacity);

    virtual ~Transport();

    void addPassengers(std::uint8_t cart, std::uint16_t passengers);
    void removePassengers(std::uint8_t cart, std::uint16_t passengers);

    std::uint16_t passengers(std::uint8_t cart) const;
    std::uint32_t totalPassengers() const;

    std::uint8_t numberOfCarts() const;
    std::uint16_t cartCapacity() const;

};

#endif