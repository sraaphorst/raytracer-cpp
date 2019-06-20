/**
 * common.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <numeric>
#include <type_traits>
#include <utility>

#include "constmath.h"

#define EPSILON 1e-4

namespace raytracer {
    template<typename S, typename T>
    constexpr typename std::enable_if_t<std::is_floating_point_v<S> || std::is_floating_point_v<T>, bool>
    ALMOST_EQUALS(S x, T y) {
        // We need special treatment for infinities here.
        if (x == std::numeric_limits<double>::infinity() && y == std::numeric_limits<double>::infinity())
            return true;
        if (x == -std::numeric_limits<double>::infinity() && y == -std::numeric_limits<double>::infinity())
            return true;
        return (const_absd(x - y) < EPSILON);
    }

    template<typename S, typename T>
    constexpr typename std::enable_if_t<std::is_floating_point_v<S> || std::is_floating_point_v<T>, bool>
    GREATER_THAN(S x, T y) {
        return x >= y - EPSILON;
    }

    template<typename S, typename T>
    constexpr typename std::enable_if_t<std::is_floating_point_v<S> || std::is_floating_point_v<T>, bool>
    LESS_THAN(S x, T y) {

        return x <= y + EPSILON;
    }
}
