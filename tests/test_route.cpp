#include <catch2/catch_test_macros.hpp>
#include "Route.h"

TEST_CASE("Route class is handled correctly"){
    Route route("B1");

    SECTION("New route is named correctly") {
        REQUIRE(route.getBusName() == "B1");
    }

    SECTION("New route is empty") {
        REQUIRE(route.getStops().empty());
    }
}