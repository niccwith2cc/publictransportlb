#include "Transport.h"

Transport::Transport(std::uint8_t carts, std::uint16_t cartCapacity)
    : _numberOfCarts(carts),
      _passengersList(carts, 0),
      _cartCapacity(cartCapacity)
{
}

Transport::~Transport() = default;

void Transport::addPassengers(std::uint8_t cart, std::uint16_t passengers)
{
    if (cart >= _numberOfCarts)
        return;

    // TODO: fix this to improve handling of overflow passengers
    if (_passengersList[cart] + passengers > _cartCapacity)
        _passengersList[cart] = _cartCapacity;
    else
        _passengersList[cart] += passengers;
}

void Transport::removePassengers(std::uint8_t cart, std::uint16_t passengers)
{
    if (cart >= _numberOfCarts)
        return;

    if (passengers >= _passengersList[cart])
        _passengersList[cart] = 0;
    else
        _passengersList[cart] -= passengers;
}

std::uint16_t Transport::passengers(std::uint8_t cart) const
{
    return _passengersList.at(cart);
}

std::uint32_t Transport::totalPassengers() const
{
    std::uint32_t total = 0;

    for (std::uint16_t p : _passengersList)
        total += p;

    return total;
}

std::uint8_t Transport::numberOfCarts() const
{
    return _numberOfCarts;
}

std::uint16_t Transport::cartCapacity() const
{
    return _cartCapacity;
}