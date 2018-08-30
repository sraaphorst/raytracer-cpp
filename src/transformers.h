/**
 * transformers.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <array>
#include <functional>
#include <utility>

#include "common.h"

namespace raytracer::details {
    /// Reduce two arrays by applying a function for each index and combining the terms.
//    template<class Function, class Reduce, class T, class R, size_t N, size_t k>
//    struct Reducer {
//        static constexpr inline R result(Function f, Reduce r, R defaultval, const std::array<T, N> &a, const std::array<T, N> &b) {
//            return r(f(a[k-1], b[k-1]), Reducer<Function, Reduce, T, R, N, k - 1>::result(f, r, defaultval, a, b));
//        }
//    };
//    template<class Function, class Reduce, class T, class R, size_t N>
//    struct Reducer<Function, Reduce, T, R, N, 0> {
//        static constexpr inline R result(Function, Reduce, R defaultval, const std::array<T, N>&, const std::array<T, N>&) {
//            return defaultval;
//        }
//    };

    template<class T, class R, size_t N, size_t k>
    struct Reducer {
        static constexpr inline R result(std::function<R(T, T)> f, std::function<R(R, R)> r, R defaultval, const std::array<T, N> &a, const std::array<T, N> &b) {
            return r(f(a[k-1], b[k-1]), Reducer<T, R, N, k - 1>::result(f, r, defaultval, a, b));
        }
    };
    template<class T, class R, size_t N>
    struct Reducer<T, R, N, 0> {
        static constexpr inline R result(std::function<R(T, T)> f, std::function<R(R, R)> r, R defaultval, const std::array<T, N>&, const std::array<T, N>&) {
            return defaultval;
        }
    };

    /// Execute a transformation on an array for each index.
    template<class R, class T, unsigned long int N, size_t... Indices>
    constexpr std::array<T, N> unitransform_helper(std::function<R(const T&)> f, const std::array<T, N> &a,
                                                   std::index_sequence<Indices...>)
    {
        return {{ f(a[Indices])... }};
    }
    template<class R, class T, unsigned long int N>
    constexpr std::array<T, N> unitransform(std::function<R(const T&)> f, const std::array<T, N> &a) {
        return unitransform_helper(f, a, std::make_index_sequence<N>{});
    }

    /// Execute a transformation on a pair of arrays for each index.
    template<class R, class T, unsigned long int N, size_t... Indices>
    constexpr std::array<T, N> bitransform_helper(std::function<R(const T&, const T&)> f, const std::array<T, N> &a1, const std::array<T, N> &a2,
                                                  std::index_sequence<Indices...>) {
        return {{ f(a1[Indices], a2[Indices])... }};
    }
    template<class R, class T, unsigned long int N>
    constexpr std::array<T, N> bitransform(std::function<R(const T&, const T&)> f, const std::array<T, N> &a1, const std::array<T, N> &a2) {
        return bitransform_helper(f, a1, a2, std::make_index_sequence<N>{});
    }

    /// Simple type specifiers.
    template<typename T, size_t N> constexpr std::array<T,N> operator+(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        return bitransform<T,T,N>([](const T &a, const T &b) { return a + b; }, t1, t2);
    }

    template<typename T, size_t N> constexpr std::array<T,N> operator-(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        return bitransform<T,T,N>([](const T &a, const T &b) { return a - b; }, t1, t2);
    }

    template<typename T, size_t N> constexpr std::array<T,N> operator*(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        return bitransform<T,T,N>([](const T &a, const T &b) { return a * b; }, t1, t2);
    }

    template<typename F, typename T, size_t N,
            typename = typename std::enable_if<std::is_arithmetic<F>::value, F>::type>
    constexpr std::array<T,N> operator*(F f, const std::array<T,N> &t) {
        return unitransform<T,T,N>([f](const T &a) { return f * a; }, t);
    }

    template<typename T, size_t N> constexpr std::array<T,N> operator/(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        return bitransform<T,T,N>([](const T &a, const T &b) { return a / b; }, t1, t2);
    }

    template<typename F, typename T, size_t N,
            typename = typename std::enable_if<std::is_arithmetic<F>::value, F>::type>
    constexpr std::array<T,N> operator/(const std::array<T,N> &t, F f) {
        return unitransform<T,T,N>([f](const T &a) { return a / f; }, t);
    }

    template<typename T, size_t N> constexpr std::array<T,N> operator==(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        return bitransform<T,T,N>([](const T &a, const T &b) { return a - b; }, t1, t2);
    }

    template<typename T, size_t N> constexpr std::array<T,N> operator-(const std::array<T,N> &t) {
        return unitransform<T,T,N>([](const T &a) { return -a; }, t);
    }

    template<typename T> constexpr bool equals(T t1, T t2) { return ALMOST_EQUALS(t1, t2); }
    template<typename T, size_t N> constexpr bool equals(const std::array<T,N> &t1, const std::array<T,N> &t2) {
        return Reducer<T, bool, N, N>::result(equals<T>, [](bool b1, bool b2) { return b1 && b2; }, true, t1, t2);
    }
}
