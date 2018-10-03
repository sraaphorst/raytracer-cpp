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
        const Tuple origin;
        const Tuple direction;

    public:
        constexpr Ray() noexcept: origin{predefined_tuples::zero_point}, direction{predefined_tuples::zero_vector} {}

        constexpr Ray(const Tuple &origin, const Tuple &direction): origin{origin}, direction{direction} {
            if (!origin.isPoint())
                throw std::invalid_argument("Ray requires a position for the first argument");
            if (!direction.isVector())
                throw std::invalid_argument("Ray requires a vector for the second argument");
        }

        constexpr Ray(const Ray &other) noexcept = default;
        constexpr Ray(Ray&& other) noexcept = default;

        // Must use parentheses instead of braces or this constructor delegates to itself.
        constexpr Ray(std::initializer_list<Tuple> lst): Ray(initializer_index(lst, 0), initializer_index(lst, 1)) {
            if (lst.size() != 2)
                throw std::length_error("Ray only accepts two parameters");
        }

        ~Ray() = default;

        constexpr Tuple position(const double t) const noexcept {
            return origin + t * direction;
        }

        constexpr const Tuple &getOrigin() const noexcept {
            return origin;
        }

        constexpr const Tuple &getDirection() const noexcept {
            return direction;
        }

        constexpr const Ray transform(const Transformation &t) const {
            return Ray{t * origin, t * direction};
        }
    };
}

