/**
 * sqrt.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <array>
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

    /// This is a bit hacky, but since we are going to call absd recursively, we need a dummy absd for arrays.

    constexpr double absd(const double d) {
        return d >= 0 ? d : -d;
    }

    template <class T>
    constexpr int numDigits(T number)
    {
        int digits = 0;
        if (number < 0) digits = 1;
        while (number) {
            number /= 10;
            digits++;
        }
        return digits;
    }
}