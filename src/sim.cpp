#include "Map.h"
#include "Tram.h"
#include "Route.h"

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
        Route route("B1");

        std::cout << "Added Route 1" << std::endl;
        // define stops with coordinates
        route.addStop("MIK", 33.8993f, 35.4851f);
        route.addStop("BIE", 33.9015f, 35.5182f);
        route.addStop("RAS", 33.8967f, 35.4741f);
        route.addStop("AUB", 33.8996f, 35.4815f);
        route.addStop("BAY", 33.8942f, 35.4984f);

        const auto& check_stops = route.getStops();
        std::cout << "Stops : ";
        for (const auto& stop : check_stops){
            std::cout << stop.name[0] << stop.name[1] << stop.name[2]
                      << " (" << stop.lat << ", " << stop.lon << "), ";
        }
        std::cout << "!" << std::endl;



    } catch (const std::exception& ex) {
        std::cerr << "Failed to load map: " << ex.what() << '\n';
        std::cerr << "See README.md for how to generate data/beirut.mapbin from a local .osm.pbf file.\n";
        return 1;
    }

    return 0;
}
