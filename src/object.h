/**
 * object.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <array>
#include <optional>

namespace raytracer {
    class Ray;

    class Object {
    protected:
        Object() = default;

    public:
        virtual const std::optional<std::array<double, 2>> intersect(const Ray&) const noexcept = 0;
    };
}
