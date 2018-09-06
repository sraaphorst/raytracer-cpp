/**
 * sqrt.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <iostream>
#include <array>
#include <cmath>
#include <limits>

namespace raytracer {
    namespace math_details {
        /// Caclulates factorials.
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


        /// Trigonometric types.
        template<typename T> constexpr T pi = 3.14159265358979323846264338327L;
        template<typename T> constexpr T two_pi = 2 * pi<T>;
        template<typename T> constexpr T half_pi = 0.5 * pi<T>;

        /// Transform val into a value between (-pi, pi).
        template<typename T>
        constexpr T normalize_radians(T val) {
            while (val < -pi<T>) val += two_pi<T>;
            while (val > pi<T>) val -= two_pi<T>;
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

    constexpr double normalize_radians(double val) {
        //return val - static_cast<int>(val / math_details::pi) * math_details::pi;
        return math_details::normalize_radians(val);
    }

    /**
     * Working to implement constexpr trigonometry operations:
     * https://codereview.stackexchange.com/questions/133668/constexpr-sin-function-c-14
     */
}