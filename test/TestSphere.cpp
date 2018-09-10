/**
 * TestSphere.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include "ray.h"
#include "sphere.h"
#include "vector.h"

using namespace raytracer;

TEST_CASE("A sphere must return a pair of points for its intersection with a ray", "[Sphere][Ray][intersect]") {
    constexpr Sphere s;
    constexpr Ray r{predefined_tuples::zero_point, predefined_tuples::zero_vector};
    
}