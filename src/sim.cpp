#include "Tram.h"
#include <iostream>

int main(){
    uint8_t carts = 1;
    uint16_t cartCapacity = 50;
    
    Tram beirutTram(carts, cartCapacity);

    std::cout << "Beirut Tram line has a capacity of: " << beirutTram.cartCapacity() << " passengers" << std::endl;
    return 0;
}