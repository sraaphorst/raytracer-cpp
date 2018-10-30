/**
 * ray.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <stdexcept>

#include <affine_transform.h>
#include <transformers.h>
#include <vec.h>

#include "ray.h"

using namespace raytracer;
using namespace raytracer::impl;

namespace raytracer::impl {
    Ray::Ray() noexcept: origin{predefined_tuples::zero_point}, direction{predefined_tuples::zero_vector} {}

    Ray::Ray(const Tuple &origin, const Tuple &direction): origin{origin}, direction{direction} {
        if (!origin.isPoint())
            throw std::invalid_argument("Ray requires a position for the first argument");
        if (!direction.isVector())
            throw std::invalid_argument("Ray requires a vector for the second argument");
    }

    bool Ray::operator==(const Ray &other) const noexcept {
        return typeid(*this) == typeid(other)
            && origin == other.origin && direction == other.direction
            && doCompare(other);
    }

    bool Ray::operator!=(const Ray &other) const noexcept {
        return !(*this == other);
    }

    Tuple Ray::position(const double t) const noexcept {
        return origin + t * direction;
    }

    const Tuple &Ray::getOrigin() const noexcept {
        return origin;
    }

    const Tuple &Ray::getDirection() const noexcept {
        return direction;
    }

    const Ray Ray::transform(const Transformation &t) const {
        return Ray{t * origin, t * direction};
    }

    bool Ray::doCompare(const Ray &other) const noexcept {
        return true;
    }
}