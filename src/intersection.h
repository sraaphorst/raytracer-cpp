/**
 * intersection.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "transformers.h"

namespace raytracer {
    using namespace transformers;

    template<typename T>
    class Intersection final {
    private:
        // Unfortunately, to make this all constexpr, we have to copy the objects instead of saving a reference or
        // pointer to them.
        const double t;
        const T o;
        Intersection() = delete;

    public:
        // We can't use an initializer_list due to mixed types, and a variadic template constructor is unnecessary.
        // This first constructor should be able to handle all the cases.
        constexpr Intersection(const double t, const T &o): t{t}, o{o} {}
        constexpr Intersection(const Intersection&) = default;
        constexpr Intersection(Intersection&&) = default;

        ~Intersection() = default;

        constexpr double getT() const noexcept {
            return t;
        }

        constexpr const T &getObject() const noexcept {
            return o;
        }
    };
}