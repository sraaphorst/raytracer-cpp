/**
 * TestLights.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include "vector.h"
#include "pointlight.h"

using namespace raytracer;

TEST_CASE("Lights: A point light has a position and intensity") {
    constexpr auto intensity = make_colour(1, 1, 1);
    constexpr auto position  = make_point(0, 0, 0);
    constexpr auto light     = PointLight(position, intensity);
    REQUIRE(light.getPosition() == position);
    REQUIRE(light.getIntensity() == intensity);
}
