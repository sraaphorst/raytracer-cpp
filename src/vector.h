/**
 * vector.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <array>
#include <initializer_list>
#include <numeric>
#include <ostream>
#include <type_traits>

#include "transformers.h"

namespace raytracer {
    using namespace transformers;

    template<typename, size_t, size_t, typename>
    class Matrix;

    template<typename T, size_t N,
            typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    class Vector {
    public:
        using type        = T;
        using vector_type = std::array<T, N>;

    protected:
        const vector_type contents;

    public:

        constexpr explicit Vector(const vector_type &contents) noexcept : contents{contents} {}
        constexpr explicit Vector(vector_type &&contents) noexcept : contents{std::move(contents)} {}
        constexpr Vector(const Vector &other) noexcept = default;
        constexpr Vector(Vector &&other) noexcept = default;

        constexpr Vector(std::initializer_list<T> lst): contents{
            make_array<T,N>([&lst](int i) { return lst.begin()[i]; })
        } {}

        ~Vector() = default;

        constexpr const T operator[](size_t idx) const noexcept {
            return contents[idx];
        }

        constexpr Vector operator+(const Vector &t) const noexcept {
            return Vector{contents + t.contents};
        }

        constexpr Vector operator-(const Vector &t) const noexcept {
            return Vector{contents - t.contents};
        }

        /// The Hadamard product.
        constexpr Vector operator*(const Vector &t) const noexcept {
            return Vector{contents * t.contents};
        }

        constexpr Vector operator*(const T t) const noexcept {
            return Vector{t * contents};
        }

        constexpr Vector operator/(const Vector &t) const noexcept {
            return Vector{contents / t.contents};
        }

        constexpr Vector operator/(const T t) const noexcept {
            return Vector{t / contents};
        }

        constexpr Vector operator-() const noexcept {
            return Vector{-contents};
        }

        constexpr bool operator==(const Vector &t) const noexcept {
            return equals(contents, t.contents);
        }

        constexpr bool operator!=(const Vector &rhs) const noexcept {
            return !(rhs == *this);
        }

        constexpr T dot_product(const Vector &t) const noexcept {
            return transformers::Reducer<T, T, N>::result(
                    [](T t1, T t2) { return t1 * t2; },
                    [](const T &t1, const T &t2) { return t1 + t2; }, 0,
                    contents, t.contents);
        }

        constexpr T magnitude() const noexcept {
            return sqrtd(dot_product(*this));
        }

        constexpr Vector normalize() const noexcept {
            return Vector{contents / magnitude()};
        }

        static constexpr size_t size() noexcept {
            return N;
        }

        friend constexpr Vector operator*(T factor, const Vector &t) noexcept {
            return t * factor;
        }

        friend std::ostream &operator<<(std::ostream &os, const Vector &t) noexcept {
            os << "Vector(";
            for (int i=0; i < t.size(); ++i)
                os << t[i] << (i == t.size() - 1 ? "" : ", ");
            return os << ')';
        }

        template<typename, size_t, size_t, typename>
        friend class Matrix;

        template<typename S, size_t m, size_t n>
        friend constexpr Vector<S,n> operator*(const Vector<S,m>&, const Matrix<S,m,n,S>&);
    };
}