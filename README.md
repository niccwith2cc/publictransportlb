# Description

C++ project about creating public transportation in Lebanon. This will cover the historical tram and a hypothetical metro station that can be used inside the country.

## Building

This project is built and managed using CMake.

### Normal workflow

```bash
cmake -B build -DBUILD_TESTS=ON
cmake --build build
./build/sim
ctest --test-dir build
```

### Regenerating the map (maintainers only)

Raw `.osm.pbf` files are **not** tracked in git. Download them locally when you need to [Geofabrik Lebanon](https://download.geofabrik.de/asia/lebanon.html).

```bash
cmake -B build -DBUILD_MAP_TOOLS=ON -DBUILD_TESTS=ON
cmake --build build
./build/pbf_converter Beirut.osm.pbf data/beirut.mapbin
```

## Sources

For more information to read on:

- [OSM Data Model](https://wiki.openstreetmap.org/wiki/Elements)
- [OSM Tags](https://wiki.openstreetmap.org/wiki/Map_features)
- [PBF File](https://wiki.openstreetmap.org/wiki/PBF_Format)
- [Libosmium](https://osmcode.org/libosmium/manual.html)