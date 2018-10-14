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
struct TestShape final: raytracer::Shape {
    // Saves the ray passed to localIntersection to make sure the proper transformation happens.
    raytracer::Ray saved_ray;

    TestShape() = default;

    template<typename T>
    explicit TestShape(T&& t): Shape{t} {}

    template<typename T, typename S>
    TestShape(T&& transformation, S&& material) noexcept: raytracer::Shape{transformation, material} {}

    TestShape(const TestShape&) noexcept = default;
    TestShape(TestShape&&) noexcept = default;
    TestShape &operator=(const TestShape&) noexcept = default;

private:
    /// Since this has no actual form, it just saves the ray to make sure it is properly translated to object space.
    const std::vector<raytracer::Intersection> localIntersection(const raytracer::Ray &r) const noexcept override {
        // This is an exceptional situation.
        const_cast<TestShape*>(this)->saved_ray = r;
        return {};
    }

    const raytracer::Tuple localNormalAt(const raytracer::Tuple &point) const noexcept override {
        return point - raytracer::predefined_tuples::zero_point;
    }
};