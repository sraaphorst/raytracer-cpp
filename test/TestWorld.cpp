/**
 * TestWorld.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <optional>

#include "affine_transform.h"
#include "material.h"
#include "pointlight.h"
#include "sphere.h"
#include "vector.h"
#include "world.h"

using namespace raytracer;

TEST_CASE("Creating a world") {
    constexpr World w;
    REQUIRE(w.getObjects().empty());
    REQUIRE(!w.getLightSource().has_value());
}

TEST_CASE("The default world") {
    constexpr PointLight light{make_point(-10, -10, -10), predefined_colours::white};

    constexpr Material m1{make_colour(0.8, 1.0, 0.6) Material::DEFAULT_AMBIENT, 0.7, 0.2, Material::DEFAULT_SHININESS};
    constexpr Sphere s1{};
    s1.setMaterial(m1);

    constexpr Sphere s2{scale(0.5, 0.5, 0.5)};

    constexpr auto w = World::getDefaultWorld();
    REQUIRE(w.getLightSource().has_value() && w.getLightSource.value() == light);
    REQUIRE(w.contains(s1));
    REQUIRE(w.contains(s2));
}