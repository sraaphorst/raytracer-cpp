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
    template<typename T>
    constexpr typename std::enable_if_t<std::is_floating_point_v<T>, bool>
    ALMOST_EQUALS(T x, T y) {
        return (absd(x - y) < EPSILON);
    }
}