/**
 * vector.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <array>
#include <numeric>
#include <ostream>
#include <type_traits>

#include "common.h"

namespace raytracer {
    namespace detail {

        /// Reduce two arrays by applying a function for each index and combining the terms.
        template<class Function, class Reduce, class T, class R, size_t N, size_t k>
        struct Reducer {
            static constexpr inline R result(Function f, Reduce r, R defaultval, const std::array<T, N> &a, const std::array<T, N> &b) {
                return r(f(a[k-1], b[k-1]), Reducer<Function, Reduce, T, R, N, k - 1>::result(f, r, defaultval, a, b));
            }
        };
        template<class Function, class Reduce, class T, class R, size_t N>
        struct Reducer<Function, Reduce, T, R, N, 0> {
            static constexpr inline R result(Function, Reduce, R defaultval, const std::array<T, N>&, const std::array<T, N>&) {
                return defaultval;
            }
        };

        /// Execute a transformation on an array for each index.
        template<class Function, class T, unsigned long int N, size_t... Indices>
        constexpr std::array<T, N> unitransform_helper(Function f, const std::array<T, N> &a,
                                                       std::index_sequence<Indices...>)
        {
            return {{ f(a[Indices])... }};
        }
        template<class Function, class T, unsigned long int N>
        constexpr std::array<T, N> unitransform(Function f, const std::array<T, N> &a) {
            return unitransform_helper(f, a, std::make_index_sequence<N>{});
        }

        /// Execute a transformation on a pair of arrays for each index.
        template<class Function, class T, unsigned long int N, size_t... Indices>
        constexpr std::array<T, N> bitransform_helper(Function f, const std::array<T, N> &a1, const std::array<T, N> &a2,
                std::index_sequence<Indices...>) {
            return {{ f(a1[Indices], a2[Indices])... }};
        }
        template<class Function, class T, unsigned long int N>
        constexpr std::array<T, N> bitransform(Function f, const std::array<T, N> &a1, const std::array<T, N> &a2) {
            return bitransform_helper(f, a1, a2, std::make_index_sequence<N>{});
        }

        template<typename T> constexpr T sum(T t1, T t2) { return t1 + t2; }
        template<typename T> constexpr T diff(T t1, T t2) { return t1 - t2; }
        template<typename T> constexpr T mult(T t1, T t2) { return t1 * t2; }
        template<typename T> constexpr T div(T t1, T t2) { return t1 / t2; }
        template<typename T> constexpr bool equals(T t1, T t2) { return ALMOST_EQUALS(t1, t2); }
        template<typename T> constexpr T neg(T t) { return -t; }
    }



    template<typename T, size_t N,
            typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    class Vector {
    public:
        using content_t = std::array<T, N>;
        using vector_t   = Vector<T,N>;

    private:
        using BiFunction = T (*)(T, T);
        using BoolBiFuncton = bool (*)(T, T);
        content_t contents;

    public:

        constexpr explicit Vector(const content_t &contents) noexcept : contents{contents} {}
        constexpr explicit Vector(content_t &&contents) noexcept : contents{std::move(contents)} {}
        constexpr Vector(const Vector &other) noexcept = default;
        constexpr Vector(Vector &&other) noexcept = default;

        /// Variadic template constructor
        template<typename... V>
        constexpr Vector(V... rs) noexcept : contents{{rs...}} {}

        constexpr const T operator[](size_t idx) const noexcept {
            return contents[idx];
        }

        constexpr vector_t operator+(const vector_t &t) const noexcept {
            return Vector{detail::bitransform(detail::sum<T>, contents, t.contents)};
        }

        constexpr vector_t operator-(const vector_t &t) const noexcept {
            return Vector{detail::bitransform(detail::diff<T>, contents, t.contents)};
        }

        /// The Hadamard product.
        constexpr vector_t operator*(const vector_t &t) const noexcept {
            return Vector{detail::bitransform(detail::mult<T>, contents, t.contents)};
        }

        constexpr vector_t operator*(const T t) const noexcept {
            return Vector(detail::unitransform([t](const T &v) { return t * v; }, contents));
        }

        constexpr vector_t operator/(const vector_t &t) const noexcept {
            return Vector{detail::bitransform(detail::div<T>, contents, t.contents)};
        }

        constexpr vector_t operator-() const noexcept {
            return Vector{detail::unitransform(detail::neg<T>, contents)};
        }

        constexpr bool operator==(const vector_t &t) const noexcept {
            return detail::Reducer<bool (*)(double, double), bool (*)(bool, bool), T, bool, N, N>::result(
                    detail::equals<T>, [](bool b1, bool b2) { return b1 && b2; }, true, contents, t.contents);
        }

        constexpr bool operator!=(const vector_t &rhs) const noexcept {
            return !(rhs == *this);
        }

        constexpr T dot_product(const vector_t &t) const noexcept {
            return detail::Reducer<BiFunction, BiFunction, T, T, N, N>::result(detail::mult<T>, detail::sum<T>, 0,
                    contents, t.contents);
        }

        constexpr T magnitude() const noexcept {
            return sqrtd(dot_product(*this));
        }

        constexpr vector_t normalize() const noexcept {
            return Vector{detail::unitransform([this](T t) { return t / magnitude(); }, contents)};
        }

        static constexpr size_t size() noexcept {
            return N;
        }

        friend constexpr vector_t operator*(T factor, const vector_t &t) {
            return t * factor;
        }

        friend std::ostream &operator<<(std::ostream &os, const vector_t &t) {
            os << "Vector(";
            for (int i=0; i < t.size(); ++i)
                os << t[i] << (i == t.size() - 1 ? "" : ", ");
            return os << ')';
        }
    };
}