/**
 * TestWorld.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <optional>

#include "affine_transform.h"
#include "hit.h"
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
    REQUIRE_FALSE(w.getLightSource().has_value());
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
    REQUIRE(xs.size() == 4);
    REQUIRE(ALMOST_EQUALS(xs[0].getT(), 4));
    REQUIRE(ALMOST_EQUALS(xs[1].getT(), 4.5));
    REQUIRE(ALMOST_EQUALS(xs[2].getT(), 5.5));
    REQUIRE(ALMOST_EQUALS(xs[3].getT(), 6));
    REQUIRE(true);
}

TEST_CASE("Shading an intersection") {
    const auto w = World::getDefaultWorld();
    const Ray ray{make_point(0, 0, -5), make_vector(0, 0, 1)};
    REQUIRE_FALSE(w.getObjects().empty());
    const auto s = w.getObjects().front();
    const Intersection i{4, s};
    const auto hit = Intersection::prepare_hit(i, ray);
    const auto cOpt = w.shade_hit(hit);
    REQUIRE(cOpt.has_value());
    const auto c = cOpt.value();
    REQUIRE(ALMOST_EQUALS(c[colour_constants::r], 0.38066));
    REQUIRE(ALMOST_EQUALS(c[colour_constants::g], 0.47583));
    REQUIRE(ALMOST_EQUALS(c[colour_constants::b], 0.2855));
}