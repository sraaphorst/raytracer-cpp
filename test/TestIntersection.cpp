/**
 * TestIntersection.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <tuple>

#include "intersection.h"
#include "intersections.h"
#include "sphere.h"
#include "transformers.h"

using namespace raytracer;
using namespace raytracer::transformers;

TEST_CASE("Intersection can be created and initialized", "[Intersection][constructor]") {
    constexpr Sphere s;
    constexpr Intersection<Sphere> it{0.5, s};
    constexpr auto s2 = it.getObject().actual();
    REQUIRE(it.getObject() == s);
    REQUIRE(it.getT() == 0.5);
}

TEST_CASE("Intersections can be aggregated", "[Intersections][Intersection][aggregate]") {
    constexpr Sphere s;
    constexpr Intersection<Sphere> i1{1, s};
    constexpr Intersection<Sphere> i2{3, s};
    constexpr auto is = Intersections::aggregate(i1, i2);
    constexpr size_t size = std::apply([](auto &&... args) { return tup_size(args...); }, is);
    REQUIRE(size == 2);

    //constexpr auto j1 = std::apply([](auto &&... args) { return tup_elem_at(0, args...); }, is);
    REQUIRE(std::get<0>(is) == i1);
    REQUIRE(std::get<1>(is) == i2);
}

// auto f = []() {
//         static int id = 0;
//         return id++;
//     };