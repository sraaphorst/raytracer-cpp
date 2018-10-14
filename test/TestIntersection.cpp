/**
 * TestIntersection.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <memory>

#include "hit.h"
#include "intersection.h"
#include "ray.h"
#include "sphere.h"
#include "transformers.h"
#include "tuple"

using namespace raytracer;
using namespace raytracer::transformers;

TEST_CASE("Intersection can be created and initialized", "[Intersection][constructor]") {
    Intersection it{0.5, std::make_shared<Sphere>()};
    REQUIRE(it.getObject() == Sphere{});
    REQUIRE(it.getT() == 0.5);
}

TEST_CASE("Intersections can be aggregated", "[Intersections][Intersection][aggregate]") {
    Intersection i1{1, std::make_shared<Sphere>()};
    Intersection i2{2, std::make_shared<Sphere>()};
    const auto is = Intersection::aggregate({i1, i2});
    REQUIRE(is.size() == 2);
    REQUIRE(is[0] == i1);
    REQUIRE(is[1] == i2);
}

TEST_CASE("Intersect sets the object on the intersection") {
    const Ray r{make_point(0, 0, -5), make_vector(0, 0, 1)};
    const Sphere s;
    const auto xs = s.intersect(r);
    REQUIRE(xs.size() == 2);
    REQUIRE(xs[0].getObject() == s);
    REQUIRE(xs[1].getObject() == s);
}

TEST_CASE("The hit, when all intersections have positive t") {
    const Intersection i1{1, std::make_shared<Sphere>()};
    const Intersection i2{2, std::make_shared<Sphere>()};
    const auto xs = Intersection::aggregate({i1, i2});
    const auto hit = Intersection::hit(xs);
    REQUIRE(hit.has_value());
    REQUIRE(hit.value() == i1);
}

TEST_CASE("The hit, when some intersections have negative t") {
    const Intersection i1{-1, std::make_shared<Sphere>()};
    const Intersection i2{1, std::make_shared<Sphere>()};
    const auto xs = Intersection::aggregate({i1, i2});
    const auto hit = Intersection::hit(xs);
    REQUIRE(hit.has_value());
    REQUIRE(hit.value() == i2);
}

TEST_CASE("The hit, when all intersections have negative t") {
    const Intersection i1{-1, std::make_shared<Sphere>()};
    const Intersection i2{-2, std::make_shared<Sphere>()};
    const auto xs = Intersection::aggregate({i1, i2});
    const auto hit = Intersection::hit(xs);
    REQUIRE_FALSE(hit.has_value());
}

TEST_CASE("The hit is always the lowest non-negative intersection") {
    const Intersection i1{5, std::make_shared<Sphere>()};
    const Intersection i2{7, std::make_shared<Sphere>()};
    const Intersection i3{-3, std::make_shared<Sphere>()};
    const Intersection i4{2, std::make_shared<Sphere>()};
    const auto xs = Intersection::aggregate({i1, i2, i3, i4});
    const auto hit = Intersection::hit(xs);
    REQUIRE(hit.has_value());
    REQUIRE(hit.value() == i4);
}

TEST_CASE("Precomputing the state of an intersection") {
    const Ray ray{make_point(0, 0, -5), make_vector(0, 0, 1)};
    const Intersection i{4, std::make_shared<const Sphere>()};
    const auto hit = Intersection::prepare_hit(i, ray);
    REQUIRE(hit.getPoint() == make_point(0, 0, -1));
    REQUIRE(hit.getEyeVector() == make_vector(0, 0, -1));
    REQUIRE(hit.getNormalVector() == make_vector(0, 0, -1));
}

TEST_CASE("An intersection occurs on the outside") {
    const Ray ray{make_point(0, 0, -5), make_vector(0, 0, 1)};
    const Intersection i{4, std::make_shared<const Sphere>()};
    const auto hit = Intersection::prepare_hit(i, ray);
    REQUIRE_FALSE(hit.isInside());
}

TEST_CASE("An intersection occurs on the inside") {
    const Ray ray{make_point(0, 0, 0), make_vector(0, 0, 1)};
    const Intersection i{1, std::make_shared<const Sphere>()};
    const auto hit = Intersection::prepare_hit(i, ray);
    REQUIRE(hit.isInside());
    REQUIRE(hit.getPoint() == make_point(0, 0, 1));
    REQUIRE(hit.getEyeVector() == make_vector(0, 0, -1));
    REQUIRE(hit.getNormalVector() == make_vector(0, 0, -1)); // inverted
}

TEST_CASE("The point is offset") {
    const Ray ray{make_point(0, 0, -5), predefined_tuples::z1};
    const Intersection i{4, std::make_shared<const Sphere>()};
    const auto hit = Intersection::prepare_hit(i, ray);
    const auto z = hit.getPoint()[tuple_constants::z];
    REQUIRE(-1.1 < z);
    REQUIRE(z < -1);
}
