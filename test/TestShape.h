/**
 * TestShape.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <vector>

#include "affine_transform.h"
#include "intersection.h"
#include "material.h"
#include "matrix.h"
#include "ray.h"
#include "shape.h"

/**
 * This is a class that implements just enough of Shape to be a concrete implementation,
 * so as to be used to isolate and test the common characteristics of all subclasses of Shape.
 */
class TestShape final: public raytracer::Shape {
public:
    TestShape() = default;

    template<typename T, typename S>
    TestShape(T&& transformation, S&& material) noexcept: raytracer::Shape{transformation, material} {}

    TestShape(const TestShape&) noexcept = default;
    TestShape(TestShape&&) noexcept = default;
    TestShape &operator=(const TestShape&) noexcept = default;

    const raytracer::Tuple normalAt(const raytracer::Tuple &p) const noexcept override {
        return raytracer::predefined_tuples::zero_vector;
    }

private:
    const std::vector<raytracer::Intersection> local_intersect(const raytracer::Ray&) const noexcept override {
        return {};
    }
};