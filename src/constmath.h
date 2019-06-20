/**
 * constmath.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <cmath>
#include <numeric>

namespace raytracer {
    namespace math_constants {
        /// Trigonometric types.
        template<typename T = double> constexpr T pi = 3.14159265358979323846264338327L;
        template<typename T = double> constexpr T two_pi = 2 * pi<T>;
        template<typename T = double> constexpr T pi_by_two = pi<T> / 2;
        template<typename T = double> constexpr T pi_by_three = pi<T> / 3;
        template<typename T = double> constexpr T pi_by_four = pi<T> / 4;
        template<typename T = double> constexpr T pi_by_six = pi<T> / 6;
        template <typename T = double> constexpr T inf = std::numeric_limits<T>::infinity();
        template <typename T = double> constexpr T ninf = -std::numeric_limits<T>::infinity();
    }

    namespace math_details {
        /// Calculates factorials.
        constexpr size_t factorial_helper(size_t sz, size_t accum) {
            if (sz == 0)
                return accum;
            else
                return factorial_helper(sz-1, sz * accum);
        }

        /// max factorial methods: could not figure out how to make this C++14 constexpr with loops, so had to revert
        /// to C++11 struct technique.
        template<size_t val, size_t i>
        struct MaxFactorial {
            static constexpr size_t value() {
                if (i > 1 && val - i * val < val) return i - 1;
                else return MaxFactorial<i * val, i + 1>::value();
            }
        };
        template<size_t val>
        struct MaxFactorial<val,2000> {
            static constexpr size_t value() { return 2000; }
        };

        /// sqrt methods.
        double constexpr sqrtNewtonRaphson(double x, double curr, double prev) {
            return curr == prev
                   ? curr
                   : sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
        }
    }

    /// constexpr sqrt of a double.
    double constexpr const_sqrtd(double x) {
        return x >= 0 && x < std::numeric_limits<double>::infinity()
               ? math_details::sqrtNewtonRaphson(x, x, 0)
               : std::numeric_limits<double>::quiet_NaN();
    }

    /// constexpr absolute value of a double.
    constexpr double const_absd(const double d) {
        return d >= 0 ? d : -d;
    }

    /// max value
    constexpr double const_maxd(const double d1, const double d2) {
        return d1 > d2 ? d1 : d2;
    }

    /// min value
    constexpr double const_mind(const double d1, const double d2) {
        return d1 < d2 ? d1 : d2;
    }

    /// constexpr calculate the number of digits.
    template <class T>
    constexpr std::enable_if_t<std::is_arithmetic_v<T>, T>
    const_numDigits(T number)
    {
        int digits = 0;
        if (number < 0) digits = 1;
        while (number) {
            number /= 10;
            digits++;
        }
        return digits;
    }

    /// Calculate n!
    constexpr inline long double const_factorial(size_t const &n) {
        return math_details::factorial_helper(n, 1);
    }

    /// Return the largest factorial we can calculate.
    constexpr size_t const_max_factorial() {
        return math_details::MaxFactorial<1,1>::value();
    }

    template<typename T>
    constexpr double const_normalize_radians(T val) {
        while (val < -math_constants::pi<T>) val += math_constants::two_pi<T>;
        while (val > math_constants::pi<T>) val -= math_constants::two_pi<T>;
        return val;
    }

    /// Taylor polynomial approximation of normalized value in [-pi, pi] with 8 terms.
    template<typename T>
    constexpr std::enable_if_t<std::is_floating_point_v<T>, T>
    const_sin(T value) {
        const T x = const_normalize_radians(value);

        T numerator = x;
        T result = 0;
        for (size_t i = 0; i < 8; ++i) {
            result += numerator / const_factorial(2 * i + 1);
            numerator *= -1 * x * x;
        }
        return result;
    }

    template<typename T>
    constexpr std::enable_if_t<std::is_floating_point_v<T>, T>
    const_cos(T value) {
        const T x = const_normalize_radians(value);

        T numerator = 1;
        T result = 0;
        for (size_t i = 0; i < 8; ++i) {
            result += numerator / const_factorial(2 * i);
            numerator *= -1 * x * x;
        }
        return result;
    }

    template<typename T>
    constexpr std::enable_if_t<std::is_floating_point_v<T>, T>
    const_tan(T value) {
        return const_sin(value) / const_cos(value);
    }

    template<typename T>
    constexpr std::enable_if_t<std::is_floating_point_v<T>, T>
    const_deg2rad(T deg) {
        return const_normalize_radians((deg * math_constants::pi<T>) / 180);
    }

    namespace math_constants {
        constexpr double sqrt2 = const_sqrtd(2.0);
        constexpr double sqrt2_by_2 = sqrt2 / 2;
    }
}