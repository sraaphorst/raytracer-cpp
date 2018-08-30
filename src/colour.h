/**
 * colour.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "vector.h"

namespace raytracer {
    /**
     * A subclass of Vector to limit it to three values, i.e. RGB, to represent Colours.
     */
    class Colour final: public Vector<double, 3> {
    public:
        constexpr explicit Colour(const vector_type &contents) noexcept: Vector{contents} {}
        constexpr explicit Colour(vector_type&& contents): Vector{contents} {}
        constexpr Colour(const Colour &other) = default;
        constexpr Colour(Colour&& other) = default;
        constexpr Colour(double rc, double gc, double bc): Vector{rc, gc, bc} {}

        /// Variadic template constructor
        template<typename... V>
        constexpr Colour(V... rs) noexcept : Vector{rs...} {}

        /// Conversion constructors: we need to be able to convert from tup to Colour to use the tup ops on Colour.
        constexpr Colour(const Vector &v): Vector{v} {}
        constexpr Colour(Vector&& v): Vector{v} {}


        /// A Colour is valid if the RGB entries are in [0,1] and the w coordinate is 0.
        constexpr bool isValid() const {
            return 0 <= (*this)[r] && (*this)[r] <= 1
                && 0 <= (*this)[g] && (*this)[g] <= 1
                && 0 <= (*this)[b] && (*this)[b] <= 1;
        }

        static constexpr int r = 0;
        static constexpr int g = 1;
        static constexpr int b = 2;
        static constexpr int maxvalue = 255;
    };

    struct colour_constants {
        static constexpr Colour black{0, 0, 0};
        static constexpr Colour white{1, 1, 1};
        static constexpr Colour red  {1, 0, 0};
        static constexpr Colour green{0, 1, 0};
        static constexpr Colour blue {0, 0, 1};
    };
}
