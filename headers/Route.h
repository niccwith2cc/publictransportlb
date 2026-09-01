#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include <array>
#include <string_view>
#include <algorithm>
#include <cstdint>

struct Stop {
    std::array<char, 3> name;
    float lat{0.0f};
    float lon{0.0f};

    bool operator==(const Stop& other) const {
        return name == other.name;
    }
};

class Route{
private:
    std::array<char, 3> _busName;                              // way to find out what bus route this is. ex: B1, ML2
    std::vector<Stop> _stops;                                  // vector of stops with 3-char names and coordinates

public:
    Route(std::string_view busName);
    Route(std::string_view busName, const std::vector<Stop>& stops);
    ~Route() = default;

    void addStop(std::string_view stopName, float lat = 0.0f, float lon = 0.0f);
    void addStop(const Stop& stop);
    void removeStop(std::string_view stopName);
    
    std::string_view getBusName() const;
    const std::vector<Stop>& getStops() const;
};

#endif