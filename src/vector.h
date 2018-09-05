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

        constexpr Vector(std::initializer_list<T> lst) : contents{initializer_list_to_array<T,N>(lst)} {}

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

        /// Tested for constexpr.
        constexpr bool operator==(const Vector &t) const noexcept {
            for (auto it1 = contents.cbegin(), it2 = t.contents.cbegin(); it1 != contents.cend(); ++it1, ++it2)
                if (!ALMOST_EQUALS(*it1, *it2))
                    return false;
            return true;
        }

        constexpr bool operator!=(const Vector &rhs) const noexcept {
            return !(rhs == *this);
        }

        constexpr T dot_product(const Vector &v) const noexcept {
            return transformer_details::dot_product<T, N>(contents, v.contents);
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

        /******************
         * TUPLE SPECIFIC *
         ******************/

        /// Cross product
        /// Seems like we need a template declaration of enable_if here. If we do a type declaration, the compiler
        /// complains that std::enable_if has no ::type.
        template<typename = typename std::enable_if_t<are_equal_v<N,4>>>
        constexpr Vector cross_product(const Vector &other) const noexcept {
            return Vector{
                    (*this)[y] * other[z] - (*this)[z] * other[y],
                    (*this)[z] * other[x] - (*this)[x] * other[z],
                    (*this)[x] * other[y] - (*this)[y] * other[x],
                    0
            };
        }

        constexpr static int x = 0;
        constexpr static int y = 0;
        constexpr static int z = 0;
        constexpr static int w = 0;
    };
}