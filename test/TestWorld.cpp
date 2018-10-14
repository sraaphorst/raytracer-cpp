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
    const Ray ray{make_point(0, 0, -5), make_vector(0, 0, 1)};
    auto xs = w.intersect(ray);
    REQUIRE(xs.size() == 4);
    REQUIRE(ALMOST_EQUALS(xs[0].getT(), 4));
    REQUIRE(ALMOST_EQUALS(xs[1].getT(), 4.5));
    REQUIRE(ALMOST_EQUALS(xs[2].getT(), 5.5));
    REQUIRE(ALMOST_EQUALS(xs[3].getT(), 6));
    REQUIRE(true);
}

TEST_CASE("Shading an intersection") {
    auto w = World::getDefaultWorld();
    const Ray ray{make_point(0, 0, -5), make_vector(0, 0, 1)};
    REQUIRE_FALSE(w.getObjects().empty());

    const auto s = w.getObjects().front();
    const Intersection i{4, s};
    auto hit = Intersection::prepare_hit(i, ray);
    auto cOpt = w.shade_hit(hit);
    REQUIRE(cOpt.has_value());
    REQUIRE(cOpt.value() == make_colour(0.38066, 0.47583, 0.2855));
}

TEST_CASE("Shading an intersection from the inside") {
    auto w = World::getDefaultWorld();
    w.setLightSource(PointLight{make_point(0, 0.25, 0), predefined_colours::white});
    const Ray ray{make_point(0, 0, 0), make_vector(0, 0, 1)};
    REQUIRE(w.getObjects().size() >= 2);

    const auto s = w.getObjects()[1];
    const Intersection i{0.5, s};
    auto hit = Intersection::prepare_hit(i, ray);
    auto cOpt = w.shade_hit(hit);
    REQUIRE(cOpt.has_value());

    // After shadowing is implemented, we only have the ambient component.
    //REQUIRE(cOpt.value() == make_colour(0.90498, 0.90498, 0.90498));
    REQUIRE(cOpt.value() == make_colour(0.1, 0.1, 0.1));
}

TEST_CASE("The colour when a ray misses") {
    const auto w = World::getDefaultWorld();
    const Ray ray{make_point(0, 0, -5), make_vector(0, 1, 0)};
    const auto c = w.colour_at(ray);
    REQUIRE(c == predefined_colours::black);
}

TEST_CASE("The colour when a ray hits") {
    const auto w = World::getDefaultWorld();
    const Ray ray{make_point(0, 0, -5), make_vector(0, 0, 1)};
    const auto c = w.colour_at(ray);
    REQUIRE(c == make_colour(0.38066, 0.47583, 0.2855));
}

// This test case is incorrect: pg 101 from the PDF.
//TEST_CASE("The colour with an intersection behind the ray") {
//    auto w = World::getDefaultWorld();
//    REQUIRE(w.getObjects().size() >= 2);
//
//    auto outer = w.getObjects()[0];
//    auto mouter = outer->getMaterial();
//    mouter.setAmbient(1);
//    outer->setMaterial(mouter);
//
//    auto inner = w.getObjects()[1];
//    auto minner = inner->getMaterial();
//    minner.setAmbient(1);
//    inner->setMaterial(minner);
//
//    REQUIRE(w.getObjects()[0]->getMaterial().getAmbient() == 1);
//    REQUIRE(w.getObjects()[1]->getMaterial().getAmbient() == 1);
//    Ray ray{make_point(0, 0, -0.75), make_vector(0, 0, 1)};
//    REQUIRE(w.colour_at(ray) == inner->getMaterial().getColour());
//}

TEST_CASE("There is no shadow when nothing is collinear with point and light") {
    const auto w = World::getDefaultWorld();
    constexpr auto p = make_point(0, 10, 0);
    REQUIRE_FALSE(w.is_shadowed(p));
}

TEST_CASE("Shadow when an object is between the point and the light") {
    const auto w = World::getDefaultWorld();
    constexpr auto p = make_point(10, -10, 10);
    REQUIRE(w.is_shadowed(p));
}

TEST_CASE("There is no shadow when an object is behind the light") {
    const auto w = World::getDefaultWorld();
    constexpr auto p = make_point(-20, 20, -20);
    REQUIRE_FALSE(w.is_shadowed(p));
}

TEST_CASE("There is no shadow when an object is behind the point") {
    const auto w = World::getDefaultWorld();
    constexpr auto p = make_point(-2, -2, -2);
    REQUIRE_FALSE(w.is_shadowed(p));
}

TEST_CASE("When shade_hit is given an intersection in shadow") {
    const PointLight light{make_point(0, 0, -10), predefined_colours::white};
    const std::shared_ptr<Shape> s1{std::make_shared<Sphere>()};
    const std::shared_ptr<Shape> s2{std::make_shared<Sphere>(translation(0, 0, 10))};
    const std::vector<std::shared_ptr<Shape>> shapes = {s1, s2};
    const World w{light, shapes};
    const Ray ray{make_point(0, 0, 5), predefined_tuples::z1};
    const Intersection i{4, s2};
    const auto hit = Intersection::prepare_hit(i, ray);
    const auto c = w.shade_hit(hit);
    for (size_t x = 0; x < 3; ++x)
        std::cout << " " << c.value()[x];
    std::cout << std::endl;
    REQUIRE(c.has_value());
    REQUIRE(c.value() == make_colour(0.1, 0.1, 0.1));
}
