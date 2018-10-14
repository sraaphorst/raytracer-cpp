/**
 * ray.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <cassert>
#include <initializer_list>
#include <stdexcept>

#include "transformers.h"
#include "vector.h"
#include "affine_transform.h"

namespace raytracer {
    using namespace transformers;

    class Ray final {
    private:
        Tuple origin;
        Tuple direction;

    public:
        Ray() noexcept: origin{predefined_tuples::zero_point}, direction{predefined_tuples::zero_vector} {}

        Ray(const Tuple &origin, const Tuple &direction): origin{origin}, direction{direction} {
            if (!origin.isPoint())
                throw std::invalid_argument("Ray requires a position for the first argument");
            if (!direction.isVector())
                throw std::invalid_argument("Ray requires a vector for the second argument");
        }

        Ray(const Ray&) noexcept = default;
        Ray(Ray&&) noexcept = default;

        Ray &operator=(const Ray&) = default;

        Tuple position(const double t) const noexcept {
            return origin + t * direction;
        }

        const Tuple &getOrigin() const noexcept {
            return origin;
        }

        const Tuple &getDirection() const noexcept {
            return direction;
        }

        const Ray transform(const Transformation &t) const {
            return Ray{t * origin, t * direction};
        }
    };
}
