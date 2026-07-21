#ifndef MAP_H
#define MAP_H

#include <cstdint>
#include <string>
#include <vector>

constexpr char MAPBIN_MAGIC[4] = {'P', 'T', 'L', 'B'};
constexpr std::uint16_t MAPBIN_VERSION = 1;

enum class RoadClass : std::uint8_t {
    Other = 0,
    Motorway = 1,
    Primary = 2,
    Secondary = 3,
    Residential = 4,
    Tram = 5,
    Metro = 6,
    LightRail = 7
};

struct MapNode {
    std::uint64_t osm_id;
    float lat;
    float lon;
};

struct MapWay {
    std::uint64_t osm_id;
    RoadClass road_class;
    std::vector<std::uint32_t> node_indices;
};

class Map {
public:
    static Map loadFromFile(const std::string& path);
    static Map fromParts(std::vector<MapNode> nodes, std::vector<MapWay> ways);

    const std::vector<MapNode>& nodes() const { return _nodes; }
    const std::vector<MapWay>& ways() const { return _ways; }

    void writeToFile(const std::string& path) const;

private:
    std::vector<MapNode> _nodes;
    std::vector<MapWay> _ways;
};

#endif
