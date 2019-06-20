/**
 * cone.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <algorithm>
#include <memory>
#include <numeric>
#include <vector>

#include "bounding_box.h"
#include "cone.h"
#include "constmath.h"
#include "intersection.h"
#include "ray.h"
#include "shape.h"
#include "vec.h"

using namespace raytracer::impl;

namespace raytracer::shapes {
    Cone::Cone(dummy d) noexcept:
            Shape{d},
            minY{-std::numeric_limits<double>::infinity()},
            maxY{ std::numeric_limits<double>::infinity()},
            capped{false} {}

    std::shared_ptr<Cone> Cone::createCone() noexcept {
        auto cone = std::make_shared<Cone>(dummy{});
        registerInstance(cone);
        return cone;
    }

    double Cone::getMinimumY() const noexcept {
        return minY;
    }

    void Cone::setMinimumY(double y) noexcept {
        minY = y;
    }

    double Cone::getMaximumY() const noexcept {
        return maxY;
    }

    void Cone::setMaximumY(double y) noexcept {
        maxY = y;
    }

    bool Cone::isCapped() const noexcept {
        return capped;
    }

    void Cone::setCapped(bool c) noexcept {
        capped = c;
    }

    bool Cone::checkCap(const impl::Ray &ray, double t, double y) const noexcept {
        const auto x = ray.getOrigin()[tuple_constants::x] + t * ray.getDirection()[tuple_constants::x];
        const auto z = ray.getOrigin()[tuple_constants::z] + t * ray.getDirection()[tuple_constants::z];
        return x * x + z * z <= y * y;
    }

    BoundingBox Cone::bounds() const {
        const auto a = const_absd(minY);
        const auto b = const_absd(maxY);
        const auto limit = const_maxd(a, b);
        return BoundingBox{make_point(-limit, minY, -limit), make_point(limit, maxY, limit)};
    }

    const std::vector<Intersection> Cone::localIntersection(const Ray &ray) const noexcept {
        const auto rdx = ray.getDirection()[tuple_constants::x];
        const auto rdy = ray.getDirection()[tuple_constants::y];
        const auto rdz = ray.getDirection()[tuple_constants::z];
        const auto rox = ray.getOrigin()[tuple_constants::x];
        const auto roy = ray.getOrigin()[tuple_constants::y];
        const auto roz = ray.getOrigin()[tuple_constants::z];

        const auto a = rdx * rdx - rdy * rdy + rdz * rdz;
        const auto b = 2 * rox * rdx - 2 * roy * rdy + 2 * roz * rdz;
        const auto c = rox * rox + - roy * roy + roz * roz;

        std::vector<Intersection> xs;

        if (ALMOST_EQUALS(a, 0) && !ALMOST_EQUALS(b, 0)) {
            const auto t = -c / 2 * b;
            xs.emplace_back(Intersection{t, shared_from_this()});
        }

        // Possible intersection with walls if a is nearly zero.
        if (!ALMOST_EQUALS(a, 0)) {
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
            if (checkCap(ray, t0, minY))
                xs.emplace_back(Intersection{t0, shared_from_this()});
            const auto t1 = (maxY - ray.getOrigin()[tuple_constants::y]) / ray.getDirection()[tuple_constants::y];
            if (checkCap(ray, t1, maxY))
                xs.emplace_back(t1, shared_from_this());
        }

        return xs;
    }


    const Tuple Cone::localNormalAt(const Tuple &point) const noexcept {
        const auto x = point[tuple_constants::x];
        const auto z = point[tuple_constants::z];

        const auto dist = x * x + z * z;
        if (dist < 1 && GREATER_THAN(point[tuple_constants::y], maxY))
            return predefined_tuples::y1;
        else if (dist < 1 && LESS_THAN(point[tuple_constants::y], minY))
            return -predefined_tuples::y1;

        auto y = const_sqrtd(x * x + z * z);
        if (point[tuple_constants::y] > 0)
            y = - y;
        return make_vector(x, y, z);
    }
}