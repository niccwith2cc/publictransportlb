#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "Route.h"

TEST_CASE("Route class is handled correctly"){
    Route route("B1");

    SECTION("New route is named correctly") {
        REQUIRE(route.getBusName() == "B1");
    }

    SECTION("New route is empty") {
        REQUIRE(route.getStops().empty());
    }

    SECTION("Adding stops with coordinates") {
        route.addStop("HAM", 33.8967f, 35.4812f);
        route.addStop("AUB", 33.8996f, 35.4815f);

        REQUIRE(route.getStops().size() == 2);
        REQUIRE(route.getStops()[0].name[0] == 'H');
        REQUIRE(route.getStops()[0].name[1] == 'A');
        REQUIRE(route.getStops()[0].name[2] == 'M');
        REQUIRE(route.getStops()[0].lat == Catch::Approx(33.8967f));
        REQUIRE(route.getStops()[0].lon == Catch::Approx(35.4812f));

        route.removeStop("HAM");
        REQUIRE(route.getStops().size() == 1);
        REQUIRE(route.getStops()[0].name[0] == 'A');
    }
}