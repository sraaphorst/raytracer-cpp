/**
 * intersection.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "object.h"
#include "transformers.h"

namespace raytracer {
    using namespace raytracer::transformers;

    class Intersection final {
    private:
        const double t;
        const Object * const o;
        Intersection() = delete;

    public:
        // We can't use an initializer_list due to mixed types, and a variadic template constructor is unnecessary.
        // This first constructor should be able to handle all the cases.
        constexpr Intersection(const double t, const Object * const o): t{t}, o{o} {}
        constexpr Intersection(const Intersection&) = default;
        constexpr Intersection(Intersection&&) = default;

        ~Intersection() = default;

        constexpr double getT() const noexcept {
            return t;
        }

        constexpr const Object* const getObject() const noexcept {
            return o;
        }
    };
}