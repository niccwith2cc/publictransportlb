#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include "Map.h"

#include <fstream>
#include <vector>

TEST_CASE("Map loader reads beirut_mini fixture", "[map]") {
    const Map map = Map::loadFromFile("tests/fixtures/beirut_mini.mapbin");

    REQUIRE(map.nodes().size() == 3);
    REQUIRE(map.ways().size() == 1);

    REQUIRE(map.nodes()[0].osm_id == 100);
    REQUIRE(map.nodes()[0].lat == Catch::Approx(33.9f));
    REQUIRE(map.nodes()[0].lon == Catch::Approx(35.5f));

    REQUIRE(map.nodes()[2].osm_id == 102);
    REQUIRE(map.ways()[0].osm_id == 200);
    REQUIRE(map.ways()[0].road_class == RoadClass::Residential);
    REQUIRE(map.ways()[0].node_indices == std::vector<std::uint32_t>{0, 1, 2});
}

TEST_CASE("Map loader rejects invalid magic", "[map]") {
    Map map;
    std::vector<MapNode> nodes{{1, 33.0f, 35.0f}, {2, 33.1f, 35.1f}};
    std::vector<MapWay> ways{{10, RoadClass::Primary, {0, 1}}};
    map = Map::fromParts(std::move(nodes), std::move(ways));
    map.writeToFile("/tmp/invalid_magic.mapbin");

    std::ifstream file("/tmp/invalid_magic.mapbin", std::ios::binary);
    std::vector<char> bytes((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    REQUIRE(bytes.size() >= 4);
    bytes[0] = 'X';
    std::ofstream out("/tmp/invalid_magic.mapbin", std::ios::binary | std::ios::trunc);
    out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    out.close();

    REQUIRE_THROWS_AS(Map::loadFromFile("/tmp/invalid_magic.mapbin"), std::runtime_error);
}

TEST_CASE("Map round-trip preserves nodes and ways", "[map]") {
    std::vector<MapNode> nodes{
        {100, 33.9f, 35.5f},
        {101, 33.91f, 35.51f},
        {102, 33.92f, 35.52f},
    };
    std::vector<MapWay> ways{
        {200, RoadClass::Residential, {0, 1, 2}},
    };

    Map original = Map::fromParts(nodes, ways);
    original.writeToFile("/tmp/roundtrip.mapbin");

    const Map loaded = Map::loadFromFile("/tmp/roundtrip.mapbin");
    REQUIRE(loaded.nodes().size() == 3);
    REQUIRE(loaded.ways().size() == 1);
    REQUIRE(loaded.ways()[0].node_indices == std::vector<std::uint32_t>{0, 1, 2});
}
