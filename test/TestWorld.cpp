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
#include "ray.h"
#include "sphere.h"
#include "vector.h"
#include "world.h"

#include <iostream>

using namespace raytracer;

TEST_CASE("Creating a world") {
    World w;
    REQUIRE(w.getObjects().empty());
    REQUIRE(!w.getLightSource().has_value());
}

TEST_CASE("The default world") {
    constexpr PointLight light{make_point(-10, 10, -10), predefined_colours::white};

    constexpr Material m1{make_colour(0.8, 1.0, 0.6), Material::DEFAULT_AMBIENT, 0.7, 0.2, Material::DEFAULT_SHININESS};
    Sphere s1{};
    s1.setMaterial(m1);

    Sphere s2{scale(0.5, 0.5, 0.5)};

    auto w = World::getDefaultWorld();
    REQUIRE(w.getLightSource().has_value());
    REQUIRE(w.getLightSource().value() == light);
    REQUIRE(w.contains(s1));
    REQUIRE(w.contains(s2));
}

TEST_CASE("Intersect a world with a ray") {
    auto w = World::getDefaultWorld();
    constexpr Ray ray{make_point(0, 0, -5), make_vector(0, 0, 1)};
    auto xs = w.intersect(ray);
    for (const auto i: xs)
        std::cout << i.getT() << '\n';
    const auto &a1 = xs[0];
    const auto &a2 = xs[1];
    const auto &a3 = xs[2];
    const auto &a4 = xs[3];
    REQUIRE(xs.size() == 4);
    REQUIRE(ALMOST_EQUALS(xs[0].getT(), 4));
    REQUIRE(ALMOST_EQUALS(xs[1].getT(), 4.5));
    REQUIRE(ALMOST_EQUALS(xs[2].getT(), 5.5));
    REQUIRE(ALMOST_EQUALS(xs[3].getT(), 6));
    REQUIRE(true);
}