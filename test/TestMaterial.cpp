/**
 * TestMaterial.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include "material.h"

using namespace raytracer;

TEST_CASE("The default material") {
    constexpr Material m;
    REQUIRE(m.getColour() == make_colour(1, 1, 1));
    REQUIRE(ALMOST_EQUALS(m.getAmbient(), 0.1));
    REQUIRE(ALMOST_EQUALS(m.getDiffuse(), 0.9));
    REQUIRE(ALMOST_EQUALS(m.getSpecular(), 0.9));
    REQUIRE(m.getShininess() == 200);
}