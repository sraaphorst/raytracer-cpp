/**
 * common.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <type_traits>
#include <utility>

#include "constmath.h"

#define EPSILON 1e-5

namespace raytracer {
    template<typename S, typename T>
    constexpr typename std::enable_if_t<std::is_floating_point_v<S> || std::is_floating_point_v<T>, bool>
    ALMOST_EQUALS(S x, T y) {
        return (absd(x - y) < EPSILON);
    }
}
