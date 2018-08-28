/**
 * sqrt.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <limits>

namespace raytracer {
    namespace {
        double constexpr sqrtNewtonRaphson(double x, double curr, double prev) {
            return curr == prev
                   ? curr
                   : sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
        }
    }

    double constexpr sqrtd(double x) {
        return x >= 0 && x < std::numeric_limits<double>::infinity()
               ? sqrtNewtonRaphson(x, x, 0)
               : std::numeric_limits<double>::quiet_NaN();
    }
}