/**
 * colour.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <initializer_list>

#include "tup.h"
#include "init_list.h"

namespace raytracer {
    using colour_contents = std::array<double, 3>;

    /**
     * A modification of tup to limit it to three values, i.e. RGB, to represent colours.
     */
    class colour final: public tup {
    public:
        constexpr explicit colour(const colour_contents &contents): tup{tup_contents{contents[r], contents[g], contents[b], 0}} {}
        constexpr explicit colour(colour_contents&& contents): tup{tup_contents{contents[r], contents[g], contents[b], 0}} {}
        constexpr colour(const colour &other) = default;
        constexpr colour(colour&& other) = default;
        constexpr colour(const std::initializer_list<double> lst): tup{idx(lst)[0], idx(lst)[1], idx(lst)[2], 0} {}
        constexpr colour(double rc, double gc, double bc): tup{rc, gc, bc, 0} {}

        /// Conversion constructors: we need to be able to convert from tup to colour to use the tup ops on colour.
        constexpr colour(const tup &t): tup{t} {}
        constexpr colour(tup&& t): tup{t} {}

        template<size_t N>
        constexpr explicit colour(const double (&arr)[N]): tup{{arr[0], arr[1], arr[2], 0}} {
            static_assert(N == 3);
        }

        /// A colour is valid if the RGB entries are in [0,1] and the w coordinate is 0.
        constexpr bool isValid() const {
            return 0 <= (*this)[r] && (*this)[r] <= 1
                && 0 <= (*this)[g] && (*this)[g] <= 1
                && 0 <= (*this)[b] && (*this)[b] <= 1
                && (*this)[w] == 0;
        }

        static constexpr int r = 0;
        static constexpr int g = 1;
        static constexpr int b = 2;
        static constexpr int maxvalue = 255;
    };

    struct colour_constants {
        static constexpr colour black{0, 0, 0};
        static constexpr colour white{1, 1, 1};
        static constexpr colour red  {1, 0, 0};
        static constexpr colour green{0, 1, 0};
        static constexpr colour blue {0, 0, 1};
    };
}
