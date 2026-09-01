#include "Map.h"

#include <osmium/handler.hpp>
#include <osmium/handler/node_locations_for_ways.hpp>
#include <osmium/index/map/sparse_mem_map.hpp>
#include <osmium/io/pbf_input.hpp>
#include <osmium/visitor.hpp>

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

namespace {

bool is_relevant_railway(const char* railway)
{
    return railway
        && (std::strcmp(railway, "tram") == 0
            || std::strcmp(railway, "subway") == 0
            || std::strcmp(railway, "light_rail") == 0);
}

bool should_keep_way(const osmium::Way& way)
{
    if (way.tags().has_key("highway"))
        return true;
    return is_relevant_railway(way.tags()["railway"]);
}

RoadClass classify_way(const osmium::Way& way)
{
    const char* railway = way.tags()["railway"];
    if (railway) {
        if (std::strcmp(railway, "tram") == 0)
            return RoadClass::Tram;
        if (std::strcmp(railway, "subway") == 0)
            return RoadClass::Metro;
        if (std::strcmp(railway, "light_rail") == 0)
            return RoadClass::LightRail;
    }

    const char* highway = way.tags()["highway"];
    if (!highway)
        return RoadClass::Other;

    if (std::strcmp(highway, "motorway") == 0 || std::strcmp(highway, "motorway_link") == 0)
        return RoadClass::Motorway;
    if (std::strcmp(highway, "primary") == 0 || std::strcmp(highway, "primary_link") == 0)
        return RoadClass::Primary;
    if (std::strcmp(highway, "secondary") == 0 || std::strcmp(highway, "secondary_link") == 0)
        return RoadClass::Secondary;
    if (std::strcmp(highway, "residential") == 0 || std::strcmp(highway, "living_street") == 0)
        return RoadClass::Residential;

    return RoadClass::Other;
}

struct StoredWay {
    osmium::object_id_type osm_id;
    RoadClass road_class;
    std::vector<osmium::object_id_type> node_refs;
};

class WayCollector : public osmium::handler::Handler {
public:
    std::uint64_t skipped_ways = 0;

    void way(const osmium::Way& way)
    {
        if (!should_keep_way(way)) {
            ++skipped_ways;
            return;
        }

        StoredWay stored;
        stored.osm_id = way.id();
        stored.road_class = classify_way(way);
        stored.node_refs.reserve(way.nodes().size());
        for (const auto& node : way.nodes())
            stored.node_refs.push_back(node.ref());

        if (stored.node_refs.size() < 2) {
            ++skipped_ways;
            return;
        }

        _ways.push_back(std::move(stored));
    }

    const std::vector<StoredWay>& ways() const { return _ways; }

private:
    std::vector<StoredWay> _ways;
};

using index_type = osmium::index::map::SparseMemMap<osmium::unsigned_object_id_type, osmium::Location>;
using location_handler_type = osmium::handler::NodeLocationsForWays<index_type>;

struct ConversionResult {
    Map map;
    std::uint64_t skipped_ways = 0;
};

ConversionResult build_map(const std::string& input_path)
{
    WayCollector collector;
    {
        osmium::io::Reader reader{input_path, osmium::osm_entity_bits::way};
        osmium::apply(reader, collector);
        reader.close();
    }

    index_type index;
    location_handler_type location_handler{index};

    {
        osmium::io::Reader reader{input_path, osmium::osm_entity_bits::node | osmium::osm_entity_bits::way};
        osmium::apply(reader, location_handler);
        reader.close();
    }

    std::unordered_map<osmium::object_id_type, std::uint32_t> node_index;
    std::vector<MapNode> nodes;
    std::vector<MapWay> ways;
    ways.reserve(collector.ways().size());

    for (const auto& stored : collector.ways()) {
        MapWay map_way;
        map_way.osm_id = static_cast<std::uint64_t>(stored.osm_id);
        map_way.road_class = stored.road_class;

        for (const auto node_ref : stored.node_refs) {
            const auto location = index.get_noexcept(node_ref);
            if (!location)
                throw std::runtime_error("missing node location for way " + std::to_string(stored.osm_id));

            auto [it, inserted] = node_index.emplace(node_ref, static_cast<std::uint32_t>(nodes.size()));
            if (inserted) {
                MapNode node;
                node.osm_id = static_cast<std::uint64_t>(node_ref);
                node.lat = static_cast<float>(location.lat());
                node.lon = static_cast<float>(location.lon());
                nodes.push_back(node);

            }

            map_way.node_indices.push_back(it->second);
        }

        ways.push_back(std::move(map_way));
    }

    return ConversionResult{Map::fromParts(std::move(nodes), std::move(ways)), collector.skipped_ways};
}

} // namespace

int main(int argc, char* argv[])
{
    if (argc != 3) {
        std::cerr << "Usage: pbf_converter <input.osm.pbf> <output.mapbin>\n";
        return 1;
    }

    const std::string input_path = argv[1];
    const std::string output_path = argv[2];

    try {
        const ConversionResult result = build_map(input_path);
        result.map.writeToFile(output_path);

        std::cout << "Converted " << input_path << " -> " << output_path << '\n';
        std::cout << "Nodes written: " << result.map.nodes().size() << '\n';
        std::cout << "Ways written: " << result.map.ways().size() << '\n';
        std::cout << "Ways skipped: " << result.skipped_ways << '\n';
    } catch (const std::exception& ex) {
        std::cerr << "Conversion failed: " << ex.what() << '\n';
        return 1;
    }

    return 0;
}
