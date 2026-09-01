#include "Route.h"
#include <cstring>
#include <stdexcept>

// Convert from typed string to be processed as individual characters for better memory efficiency
static std::array<char, 3> to_char_array(std::string_view sv) {
    std::array<char, 3> arr = {'\0', '\0', '\0'};
    std::size_t len = std::min(sv.length(), arr.size());
    std::memcpy(arr.data(), sv.data(), len);
    return arr;
}

Route::Route(std::string_view busName)
    : _busName(to_char_array(busName)) {}

Route::Route(std::string_view busName, const std::vector<Stop>& stops)
    : _busName(to_char_array(busName)), _stops(stops) {}

void Route::addStop(std::string_view stopName, float lat, float lon) {
    _stops.push_back(Stop{to_char_array(stopName), lat, lon});
}

void Route::addStop(const Stop& stop) {
    _stops.push_back(stop);
}

void Route::removeStop(std::string_view stopName) {
    auto targetName = to_char_array(stopName);
    _stops.erase(
        std::remove_if(_stops.begin(), _stops.end(), [&](const Stop& s) {
            return s.name == targetName;
        }),
        _stops.end()
    );
}
    
std::string_view Route::getBusName() const {
    std::size_t len = (_busName[2] == '\0') ? 2 : 3;
    return std::string_view(_busName.data(), len);
}

const std::vector<Stop>& Route::getStops() const {
    return _stops;
}