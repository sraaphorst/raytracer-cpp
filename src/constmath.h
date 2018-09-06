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
//        constexpr long double factorial_helper(size_t sz, long double accum) {
//            if (sz == 0)
//                return accum;
//            else
//                return factorial_helper(sz-1, sz * accum);
//        }

        constexpr size_t factorial_helper(size_t sz, size_t accum) {
            if (sz == 0)
                return accum;
            else
                return factorial_helper(sz-1, sz * accum);
        }

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

    /// 1755?
    template<size_t i>
    struct MF {
        static constexpr size_t value(long double val) {
            const auto newval = val * i;
            if (std::isinf(newval)) return i - 1;
            //if constexpr(factorial(i+1) < factorial(i)) return i-1;
            else return MF<i+1>::value(newval);
        }
    };

    template<>
    struct MF<20000> {
        static constexpr size_t value(long double) { return 20000; }
    };

    /// Return the largest factorial we can calculate.
//    constexpr size_t max_factorial() {
//        size_t i = 1;
//        while (factorial(i) < std::numeric_limits<long double>::max()) { ++i; }
//        return i-1;
//        return MF<1>::value(1.0L);
//    }

    template<size_t val, size_t i>
    struct MF2 {
        static constexpr size_t value() {
            if (i > 1 && val - i * val < val) return i - 1;
                //if constexpr(factorial(i+1) < factorial(i)) return i-1;
            else return MF2<i * val, i + 1>::value();
        }
    };

    template<size_t val>
    struct MF2<val,2000> {
        static constexpr size_t value() { return 2000; }
    };

    /// Overloads template instantiation depth.
    template<size_t i>
    constexpr size_t max_factorial_helper(const long double current) {
        std::cout << i << ": " << current << '\n';
        if (i < 200) return max_factorial_helper<i+1>(current * i);// || current < std::numeric_limits<long double>::max()) return max_factorial_helper<i+1>(current * i);
        else return i - 1;
    }

    constexpr size_t max_factorial() {
        return MF2<1,1>::value();
    }
//    constexpr size_t max_factorial() {
//        return max_factorial_helper<1>(1);
//    }

    /**
     * Working to implement constexpr trigonometry operations:
     * https://codereview.stackexchange.com/questions/133668/constexpr-sin-function-c-14
     */
}