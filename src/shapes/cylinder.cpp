/**
 * cylinder.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <algorithm>
#include <memory>
#include <vector>

#include "bounding_box.h"
#include "constmath.h"
#include "cylinder.h"
#include "intersection.h"
#include "ray.h"
#include "shape.h"
#include "vec.h"

using namespace raytracer::impl;

namespace raytracer::shapes {
    Cylinder::Cylinder(dummy d) noexcept:
        Shape{d},
        minY{math_constants::ninf<>},
        maxY{math_constants::inf<>},
        capped{false} {}

    std::shared_ptr<Cylinder> Cylinder::createCylinder() noexcept {
        auto cylinder = std::make_shared<Cylinder>(dummy{});
        registerInstance(cylinder);
        return cylinder;
    }

    double Cylinder::getMinimumY() const noexcept {
        return minY;
    }

    void Cylinder::setMinimumY(double y) noexcept {
        minY = y;
    }

    double Cylinder::getMaximumY() const noexcept {
        return maxY;
    }

    void Cylinder::setMaximumY(double y) noexcept {
        maxY = y;
    }

    bool Cylinder::isCapped() const noexcept {
        return capped;
    }

    void Cylinder::setCapped(bool c) noexcept {
        capped = c;
    }

    /// Get a bounding box.
    BoundingBox Cylinder::bounds() const {
        return BoundingBox{make_point(-1, minY, -1), make_point(1, maxY, 1)};
    }

    bool Cylinder::checkCap(const impl::Ray &ray, double t) const noexcept {
        const auto x = ray.getOrigin()[tuple_constants::x] + t * ray.getDirection()[tuple_constants::x];
        const auto z = ray.getOrigin()[tuple_constants::z] + t * ray.getDirection()[tuple_constants::z];
        return x * x + z * z <= 1;
    }

    const std::vector<Intersection> Cylinder::localIntersection(const Ray &ray) const noexcept {
        const auto rdx = ray.getDirection()[tuple_constants::x];
        const auto rdz = ray.getDirection()[tuple_constants::z];
        const auto a = rdx * rdx + rdz * rdz;

        std::vector<Intersection> xs;

        // Possible intersection with walls if a is nearly zero.
        if (!ALMOST_EQUALS(a, 0)) {
            const auto rox = ray.getOrigin()[tuple_constants::x];
            const auto roz = ray.getOrigin()[tuple_constants::z];

            const auto b = 2 * rox * rdx + 2 * roz * rdz;
            const auto c = rox * rox + roz * roz - 1;

            const auto discriminant = b * b - 4 * a * c;
            if (discriminant >= 0) {
                const auto sqrt_discriminant = const_sqrtd(discriminant);
                auto t0 = (-b - sqrt_discriminant) / (2 * a);
                auto t1 = (-b + sqrt_discriminant) / (2 * a);
                if (t0 > t1)
                    std::swap(t0, t1);

                const auto y0 = ray.getOrigin()[tuple_constants::y] + t0 * ray.getDirection()[tuple_constants::y];
                if (minY < y0 && y0 < maxY)
                    xs.emplace_back(Intersection{t0, shared_from_this()});

                const auto y1 = ray.getOrigin()[tuple_constants::y] + t1 * ray.getDirection()[tuple_constants::y];
                if (minY < y1 && y1 < maxY)
                    xs.emplace_back(Intersection{t1, shared_from_this()});
            }
        }

        // Check for intersection with caps.
        if (capped && !ALMOST_EQUALS(ray.getDirection()[tuple_constants::y], 0)) {
            const auto t0 = (minY - ray.getOrigin()[tuple_constants::y]) / ray.getDirection()[tuple_constants::y];
            if (checkCap(ray, t0))
                xs.emplace_back(Intersection{t0, shared_from_this()});
            const auto t1 = (maxY - ray.getOrigin()[tuple_constants::y]) / ray.getDirection()[tuple_constants::y];
            if (checkCap(ray, t1))
                xs.emplace_back(t1, shared_from_this());
        }

        return xs;
    }


    const Tuple Cylinder::localNormalAt(const Tuple &point) const noexcept {
        const auto x = point[tuple_constants::x];
        const auto z = point[tuple_constants::z];

        const auto dist = x * x + z * z;
        if (dist < 1 && GREATER_THAN(point[tuple_constants::y], maxY))
            return predefined_tuples::y1;
        else if (dist < 1 && LESS_THAN(point[tuple_constants::y], minY))
            return -predefined_tuples::y1;
        else
            return make_vector(point[tuple_constants::x], 0, point[tuple_constants::z]);
    }
}