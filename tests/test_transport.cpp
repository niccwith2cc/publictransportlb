#include <catch2/catch_test_macros.hpp>
#include "Transport.h"

TEST_CASE("Transport class handles passengers correctly", "[transport]") {
    Transport vehicle(3, 50); // 3 carts, capacity 50 each

    SECTION("New transport starts empty") {
        REQUIRE(vehicle.totalPassengers() == 0);
        REQUIRE(vehicle.numberOfCarts() == 3);
        REQUIRE(vehicle.cartCapacity() == 50);
    }

    SECTION("Adding passengers respects capacity limits") {
        vehicle.addPassengers(0, 30);
        REQUIRE(vehicle.passengers(0) == 30);

        // Try to overfill cart 0
        vehicle.addPassengers(0, 100);
        REQUIRE(vehicle.passengers(0) == 50); // Should clamp to capacity
    }

    SECTION("Removing passengers works correctly") {
        vehicle.addPassengers(1, 40);
        vehicle.removePassengers(1, 15);
        REQUIRE(vehicle.passengers(1) == 25);

        // Try to remove more than exist
        vehicle.removePassengers(1, 100);
        REQUIRE(vehicle.passengers(1) == 0);
    }
}