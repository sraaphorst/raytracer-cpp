/**
 * TestMaterial.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include "constmath.h"
#include "material.h"
#include "pointlight.h"

using namespace raytracer;

TEST_CASE("Material: The default material") {
    const Material m;
    REQUIRE(m.getColour() == make_colour(1, 1, 1));
    REQUIRE(ALMOST_EQUALS(m.getAmbient(), 0.1));
    REQUIRE(ALMOST_EQUALS(m.getDiffuse(), 0.9));
    REQUIRE(ALMOST_EQUALS(m.getSpecular(), 0.9));
    REQUIRE(m.getShininess() == 200);
}

TEST_CASE("Material: Lighting with the eye between the light and the surface") {
    constexpr Material m;
    constexpr auto position = predefined_tuples::zero_point;
    constexpr auto eyev = make_vector(0, 0, -1);
    constexpr auto normalv = make_vector(0, 0, -1);
    constexpr PointLight light{make_point(0, 0, -10), predefined_colours::white};
    const auto result = m.lighting(light, position, eyev, normalv, false);
    REQUIRE(result == make_colour(1.9, 1.9, 1.9));
}

TEST_CASE("Material: Lighting with the eye between light and surface, eye offset 45 deg") {
    constexpr Material m;
    const auto position = predefined_tuples::zero_point;
    constexpr auto sqrt2by2 = sqrtd(2) / 2;
    constexpr auto eyev = make_vector(0, sqrt2by2, -sqrt2by2);
    constexpr auto normalv = make_vector(0, 0, -1);
    constexpr PointLight light{make_point(0, 0, -10), predefined_colours::white};
    const auto result = m.lighting(light, position, eyev, normalv, false);
    REQUIRE(result == predefined_colours::white);
}

TEST_CASE("Material: Lighting with eye opposite surface, light offset 45 deg") {
    constexpr Material m;
    constexpr auto position = predefined_tuples::zero_point;
    constexpr auto eyev = make_vector(0, 0, -1);
    constexpr auto normalv = make_vector(0, 0, -1);
    constexpr PointLight light{make_point(0, 10, -10), predefined_colours::white};
    const auto result = m.lighting(light, position, eyev, normalv, false);
    REQUIRE(result == make_colour(0.7364, 0.7364, 0.7364));
}

TEST_CASE("Material: Lighting with eye in the path of the reflection vector") {
    constexpr Material m;
    constexpr auto position = predefined_tuples::zero_point;
    constexpr auto sqrt2by2 = sqrtd(2) / 2;
    constexpr auto eyev = make_vector(0, -sqrt2by2, -sqrt2by2);
    constexpr auto normalv = make_vector(0, 0, -1);
    constexpr PointLight light{make_point(0, 10, -10), predefined_colours::white};
    const auto result = m.lighting(light, position, eyev, normalv, false);
    REQUIRE(result == make_colour(1.6364, 1.6364, 1.6364));
}

TEST_CASE("Material: Lighting with the light behind the surface") {
    constexpr Material m;
    constexpr auto position = predefined_tuples::zero_point;
    constexpr auto eyev = make_vector(0, 0, -1);
    constexpr auto normalv = make_vector(0, 0, -1);
    constexpr PointLight light{make_point(0, 0, 10), predefined_colours::white};
    const auto result = m.lighting(light, position, eyev, normalv, false);
    REQUIRE(result == make_colour(0.1, 0.1, 0.1));
}

TEST_CASE("Material: Lighting with the surface in shadow") {
    constexpr Material m;
    constexpr auto position = predefined_tuples::zero_point;
    constexpr auto eyev = -predefined_tuples::z1;
    constexpr auto normalv = -predefined_tuples::z1;
    constexpr PointLight light{make_point(0, 0, -10), predefined_colours::white};
    constexpr bool in_shadow = true;
    const auto result = m.lighting(light, position, eyev, normalv, in_shadow);
    REQUIRE(result == make_colour(0.1, 0.1, 0.1));
}
