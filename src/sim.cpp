#include "Map.h"
#include "Tram.h"

#include <iostream>
#include <stdexcept>

int main()
{
    try {
        const Map map = Map::loadFromFile("data/beirut.mapbin");
        std::cout << "Loaded Beirut map: "
                  << map.nodes().size() << " nodes, "
                  << map.ways().size() << " ways\n";

        uint8_t carts = 1;
        uint16_t cartCapacity = 50;
        Tram beirutTram(carts, cartCapacity);
        std::cout << "Beirut Tram line has a capacity of: "
                  << beirutTram.cartCapacity() << " passengers\n";
    } catch (const std::exception& ex) {
        std::cerr << "Failed to load map: " << ex.what() << '\n';
        std::cerr << "See README.md for how to generate data/beirut.mapbin from a local .osm.pbf file.\n";
        return 1;
    }

    return 0;
}
