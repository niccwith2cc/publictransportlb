#include "Map.h"

#include <fstream>
#include <stdexcept>
#include <cstring>

namespace {

void write_u16(std::ostream& out, std::uint16_t value)
{
    out.put(static_cast<char>(value & 0xFF));
    out.put(static_cast<char>((value >> 8) & 0xFF));
}

void write_u32(std::ostream& out, std::uint32_t value)
{
    for (int shift = 0; shift < 32; shift += 8)
        out.put(static_cast<char>((value >> shift) & 0xFF));
}

void write_u64(std::ostream& out, std::uint64_t value)
{
    for (int shift = 0; shift < 64; shift += 8)
        out.put(static_cast<char>((value >> shift) & 0xFF));
}

void write_f32(std::ostream& out, float value)
{
    static_assert(sizeof(float) == 4, "float must be 4 bytes");
    std::uint32_t bits = 0;
    std::memcpy(&bits, &value, sizeof(float));
    write_u32(out, bits);
}

std::uint8_t read_u8(std::istream& in)
{
    char byte = 0;
    in.get(byte);
    if (!in)
        throw std::runtime_error("unexpected end of map file");
    return static_cast<std::uint8_t>(byte);
}

std::uint16_t read_u16(std::istream& in)
{
    char bytes[2];
    in.read(bytes, 2);
    if (!in)
        throw std::runtime_error("unexpected end of map file");
    return static_cast<std::uint8_t>(bytes[0])
         | (static_cast<std::uint16_t>(static_cast<std::uint8_t>(bytes[1])) << 8);
}

std::uint32_t read_u32(std::istream& in)
{
    std::uint32_t value = 0;
    for (int shift = 0; shift < 32; shift += 8) {
        char byte = 0;
        in.get(byte);
        if (!in)
            throw std::runtime_error("unexpected end of map file");
        value |= static_cast<std::uint32_t>(static_cast<std::uint8_t>(byte)) << shift;
    }
    return value;
}

std::uint64_t read_u64(std::istream& in)
{
    std::uint64_t value = 0;
    for (int shift = 0; shift < 64; shift += 8) {
        char byte = 0;
        in.get(byte);
        if (!in)
            throw std::runtime_error("unexpected end of map file");
        value |= static_cast<std::uint64_t>(static_cast<std::uint8_t>(byte)) << shift;
    }
    return value;
}

float read_f32(std::istream& in)
{
    std::uint32_t bits = read_u32(in);
    float value = 0.0f;
    std::memcpy(&value, &bits, sizeof(float));
    return value;
}

void read_magic(std::istream& in)
{
    char magic[4];
    in.read(magic, 4);
    if (!in)
        throw std::runtime_error("map file is too small");
    if (std::memcmp(magic, MAPBIN_MAGIC, 4) != 0)
        throw std::runtime_error("invalid map file: bad magic");
}

} // namespace

Map Map::loadFromFile(const std::string& path)
{
    std::ifstream in(path, std::ios::binary);
    if (!in)
        throw std::runtime_error("failed to open map file: " + path);

    read_magic(in);

    const auto version = read_u16(in);
    if (version != MAPBIN_VERSION)
        throw std::runtime_error("unsupported map file version");

    Map map;
    const auto node_count = read_u32(in);
    const auto way_count = read_u32(in);

    map._nodes.reserve(node_count);
    for (std::uint32_t i = 0; i < node_count; ++i) {
        MapNode node;
        node.osm_id = read_u64(in);
        node.lat = read_f32(in);
        node.lon = read_f32(in);
        map._nodes.push_back(node);
    }

    map._ways.reserve(way_count);
    for (std::uint32_t i = 0; i < way_count; ++i) {
        MapWay way;
        way.osm_id = read_u64(in);
        way.road_class = static_cast<RoadClass>(read_u8(in));
        const auto node_ref_count = read_u16(in);
        way.node_indices.reserve(node_ref_count);
        for (std::uint16_t j = 0; j < node_ref_count; ++j)
            way.node_indices.push_back(read_u32(in));
        map._ways.push_back(std::move(way));
    }

    return map;
}

Map Map::fromParts(std::vector<MapNode> nodes, std::vector<MapWay> ways)
{
    Map map;
    map._nodes = std::move(nodes);
    map._ways = std::move(ways);
    return map;
}

void Map::writeToFile(const std::string& path) const
{
    std::ofstream out(path, std::ios::binary | std::ios::trunc);
    if (!out)
        throw std::runtime_error("failed to open map file for writing: " + path);

    out.write(MAPBIN_MAGIC, 4);
    write_u16(out, MAPBIN_VERSION);
    write_u32(out, static_cast<std::uint32_t>(_nodes.size()));
    write_u32(out, static_cast<std::uint32_t>(_ways.size()));

    for (const auto& node : _nodes) {
        write_u64(out, node.osm_id);
        write_f32(out, node.lat);
        write_f32(out, node.lon);
    }

    for (const auto& way : _ways) {
        write_u64(out, way.osm_id);
        out.put(static_cast<char>(static_cast<std::uint8_t>(way.road_class)));
        write_u16(out, static_cast<std::uint16_t>(way.node_indices.size()));
        for (const auto index : way.node_indices)
            write_u32(out, index);
    }
}
