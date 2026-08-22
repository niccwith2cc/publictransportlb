#ifndef ROUTE_H
#define ROUTE_H

#include <vector>
#include <array>
#include <string_view>
#include <algorithm>

class Route{
private:
    std::array<char, 3> _busName;                              // way to find out what bus route this is. ex: B1, ML2
    std::vector<std::array<char, 3>> _stopNames;               // vector of stop names abbreviated by 3 characters ex: BAY  
    //TODO: add bus stop locations
public:
    Route(std::string_view busName);
    Route(std::string_view busName, const std::vector<std::array<char, 3>>& stopNames);
    ~Route() = default;

    void addStop(std::string_view stopName);
    void removeStop(std::string_view stopName);
    
    std::string_view getBusName() const;
    const std::vector<std::array<char, 3>>& getStops() const;
};

#endif