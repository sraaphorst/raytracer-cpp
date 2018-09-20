/**
 * intersections.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <tuple>

#include "intersection.h"

namespace raytracer {
    class Intersections final {
    private:
        Intersections() = delete;

    public:
        template<typename S, typename T>
        static constexpr std::tuple<S, T> aggregate(const S &s, const T &t) {
            return std::make_tuple(s, t);
        }
    };
}