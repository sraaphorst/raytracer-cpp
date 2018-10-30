/**
 * constmath.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <cmath>

namespace raytracer {
    namespace math_constants {
        /// Trigonometric types.
        template<typename T = double> constexpr T pi = 3.14159265358979323846264338327L;
        template<typename T = double> constexpr T two_pi = 2 * pi<T>;
        template<typename T = double> constexpr T pi_by_two = pi<T> / 2;
        template<typename T = double> constexpr T pi_by_three = pi<T> / 3;
        template<typename T = double> constexpr T pi_by_four = pi<T> / 4;
        template<typename T = double> constexpr T two_pi_by_three = 2 * pi<T> / 3;
        template<typename T = double> constexpr T sqrt2 = 2 * std::sqrt<T>(2);
        template<typename T = double> constexpr T sqrt2by2 = sqrt2<T> / 2;

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

        /// Inverse of a floating point number.
        template<class T,class dcy = std::decay_t<T>>
        constexpr inline std::enable_if_t<std::is_floating_point<T>::value,dcy> inverse(T value){
            return (value == 0) ? 0.0 : 1.0 / value;
        }

        /// sqrt methods.
        double constexpr sqrtNewtonRaphson(double x, double curr, double prev) {
            return curr == prev
                   ? curr
                   : sqrtNewtonRaphson(x, 0.5 * (curr + x / curr), curr);
        }

        /// Transform val into a value between (-pi, pi).
        template<typename T>
        constexpr T normalize_radians(T val) {
            while (val < -math_constants::pi<T>) val += math_constants::two_pi<T>;
            while (val > math_constants::pi<T>) val -= math_constants::two_pi<T>;
            return val;
        }

    }

    /// constexpr sqrt of a double.
    double constexpr sqrtd(double x) {
        return x >= 0 && x < std::numeric_limits<double>::infinity()
               ? math_details::sqrtNewtonRaphson(x, x, 0)
               : std::numeric_limits<double>::quiet_NaN();
    }

    /// constexpr absolute value of a double.
    constexpr double absd(const double d) {
        return d >= 0 ? d : -d;
    }

    /// constexpr calculate the number of digits.
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

    /// Calculate n!
    constexpr inline long double factorial(size_t const& n) {
        return math_details::factorial_helper(n, 1);
    }

    /// Return the largest factorial we can calculate.
    constexpr size_t max_factorial() {
        return math_details::MaxFactorial<1,1>::value();
    }

    template<typename T>
    constexpr double normalize_radians(T val) {
        return math_details::normalize_radians(val);
    }

    /// Taylor polynomial approximation of normalized value in [-pi, pi] with 8 terms.
    template<typename T>
    constexpr std::enable_if_t<std::is_floating_point_v<T>, T>
    sinc(T value) {
        const T x = normalize_radians(value);

        T numerator = x;
        T result = 0;
        for (size_t i = 0; i < 8; ++i) {
            result = result + numerator / factorial(2 * i + 1);
            numerator *= -1 * x * x;
        }
        return result;
    }

    template<typename T>
    constexpr std::enable_if_t<std::is_floating_point_v<T>, T>
    cosc(T value) {
        const T x = normalize_radians(value);

        T numerator = 1;
        T result = 0;
        for (size_t i = 0; i < 8; ++i) {
            result = result + numerator / factorial(2 * i);
            numerator *= -1 * x * x;
        }
        return result;
    }

    template<typename T>
    constexpr std::enable_if_t<std::is_floating_point_v<T>, T>
    tanc(T value) {
        return sinc(value) / cosc(value);
    }

    template<typename T>
    constexpr std::enable_if_t<std::is_floating_point_v<T>, T>
    deg_to_rad(T deg) {
        return normalize_radians((deg * math_constants::pi<T>) / 180);
    }
}