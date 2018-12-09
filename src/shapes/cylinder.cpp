/**
 * cylinder.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <memory>
#include <vector>

#include "constmath.h"
#include "cylinder.h"
#include "intersection.h"
#include "ray.h"
#include "shape.h"
#include "vec.h"

using namespace raytracer::impl;

namespace raytracer::shapes {
    Cylinder::Cylinder(dummy d) noexcept: Shape{d} {}

    std::shared_ptr<Cylinder> Cylinder::createCylinder() noexcept {
        auto cylinder = std::make_shared<Cylinder>(dummy{});
        registerInstance(cylinder);
        return cylinder;
    }

    const std::vector<Intersection> Cylinder::localIntersection(const Ray &ray) const noexcept {
        const auto rdx = ray.getDirection()[tuple_constants::x];
        const auto rdz = ray.getDirection()[tuple_constants::z];
        const auto a = rdx * rdx + rdz * rdz;

        // If the ray is parallel to the y axis, nothing.
        if (ALMOST_EQUALS(a, 0))
            return {};

        const auto rox = ray.getOrigin()[tuple_constants::x];
        const auto roz = ray.getOrigin()[tuple_constants::z];

        auto b = 2 * rox * rdx + 2 * roz * rdz;
        auto c = rox * rox + roz * roz - 1;

        const auto discriminant = b * b - 4 * a * c;
        if (discriminant < 0)
            return {};

        const auto sqrt_discriminant = const_sqrtd(discriminant);
        const auto t1 = (-b - sqrt_discriminant) / (2 * a);
        const auto t2 = (-b + sqrt_discriminant) / (2 * a);
        if (t1 < t2) return {Intersection{t1, shared_from_this()}, Intersection{t2, shared_from_this()}};
        else return {Intersection{t2, shared_from_this()}, Intersection{t1, shared_from_this()}};
    }

    const Tuple Cylinder::localNormalAt(const Tuple &point) const noexcept {
        return predefined_tuples::zero_vector;
    }
}