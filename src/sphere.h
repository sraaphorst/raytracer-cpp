/**
 * sphere.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <array>
#include <constmath.h>
#include <optional>
#include <tuple>
#include <vector>

#include "intersection.h"
#include "ray.h"
#include "shape.h"
#include "vector.h"

namespace raytracer {
    class Sphere final: public Shape {
    public:
        Sphere() = default;

        template<typename T>
        explicit Sphere(T&& t): Shape{t, Material{}} {}

        template<typename T, typename M>
        Sphere(T&& t, M&& m): Shape{t, m} {}

        const std::vector<Intersection> intersect(const Ray &r0) const noexcept override {
            // First transform the ray.
            const Ray r = r0.transform(getTransformation().invert());
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
            if (t1 < t2) return {Intersection{t1, *this}, Intersection{t2, *this}};
            else return {Intersection{t2, *this}, Intersection{t1, *this}};
        }

        const Tuple normalAt(const Tuple &p) const override {
            //return (p - make_point(0, 0, 0)).normalize();
            const auto object_point = transformation.invert() * p;
            const auto object_normal = object_point - predefined_tuples::zero_point;
            const auto world_normal = transformation.invert().transpose() * object_normal;
            return make_vector(world_normal[tuple_constants::x],
                    world_normal[tuple_constants::y],
                    world_normal[tuple_constants::z]).normalize();
        }

    private:
        bool doCompare(const Shape &other) const override {
            const auto *s = dynamic_cast<const Sphere*>(&other);
            return (s != nullptr);
        }
    };
}
