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

Route::Route(std::string_view busName, const std::vector<std::array<char, 3>>& stopNames)
    : _busName(to_char_array(busName)), _stopNames(stopNames) {}

void Route::addStop(std::string_view stopName) {
    _stopNames.push_back(to_char_array(stopName));
}

void Route::removeStop(std::string_view stopName) {
    auto target = to_char_array(stopName);
    _stopNames.erase(
        std::remove(_stopNames.begin(), _stopNames.end(), target),
        _stopNames.end()
    );
}
    
std::string_view Route::getBusName() const {
    std::size_t len = (_busName[2] == '\0') ? 2 : 3;
    return std::string_view(_busName.data(), len);
}

const std::vector<std::array<char, 3>>& Route::getStops() const{
    return _stopNames;
}