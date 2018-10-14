/**
 * sphere.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <memory>
#include <vector>

#include "constmath.h"
#include "intersection.h"
#include "ray.h"
#include "sphere.h"
#include "vector.h"

namespace raytracer {
    const std::vector<Intersection> Sphere::localIntersection(const Ray &r) const noexcept {
        const auto sphere_to_ray = r.getOrigin() - predefined_tuples::zero_point;
        const auto &direction = r.getDirection();

        const auto a = direction.dot_product(direction);
        const auto b = 2 * direction.dot_product(sphere_to_ray);
        const auto c = sphere_to_ray.dot_product(sphere_to_ray) - 1;

        const auto discriminant = b * b - 4 * a * c;
        if (discriminant < 0)
        return {};

        const auto t1 = (-b - sqrtd(discriminant)) / (2 * a);
        const auto t2 = (-b + sqrtd(discriminant)) / (2 * a);
        const auto ptr = std::make_shared<const Sphere>(transformation, material);
        if (t1 < t2) return {Intersection{t1, ptr}, Intersection{t2, ptr}};
        else return {Intersection{t2, ptr}, Intersection{t1, ptr}};
    }

    const Tuple Sphere::localNormalAt(const Tuple &point) const noexcept {
        return point - predefined_tuples::zero_point;
    }
}