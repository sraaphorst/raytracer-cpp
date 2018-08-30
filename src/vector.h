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

#include "transformers.h"

namespace raytracer {
    template<typename T, size_t N,
            typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
    class Vector {
    public:
        using type        = T;
        using vector_type = std::array<T, N>;

    private:
        using BiFunction = T (*)(T, T);
        using BoolBiFuncton = bool (*)(T, T);

    protected:
        const vector_type contents;

    public:

        constexpr explicit Vector(const vector_type &contents) noexcept : contents{contents} {}
        constexpr explicit Vector(vector_type &&contents) noexcept : contents{std::move(contents)} {}
        constexpr Vector(const Vector &other) noexcept = default;
        constexpr Vector(Vector &&other) noexcept = default;

        /// Variadic template constructor
        template<typename... V>
        constexpr Vector(V... rs) noexcept : contents{{rs...}} {}

        ~Vector() = default;

        constexpr const T operator[](size_t idx) const noexcept {
            return contents[idx];
        }

        constexpr Vector operator+(const Vector &t) const noexcept {
            return Vector{details::bitransform(details::sum<T>, contents, t.contents)};
        }

        constexpr Vector operator-(const Vector &t) const noexcept {
            return Vector{details::bitransform(details::diff<T>, contents, t.contents)};
        }

        /// The Hadamard product.
        constexpr Vector operator*(const Vector &t) const noexcept {
            return Vector{details::bitransform(details::mult<T>, contents, t.contents)};
        }

        constexpr Vector operator*(const T t) const noexcept {
            return Vector(details::unitransform([t](const T &v) { return t * v; }, contents));
        }

        constexpr Vector operator/(const Vector &t) const noexcept {
            return Vector{details::bitransform(details::div<T>, contents, t.contents)};
        }

        constexpr Vector operator-() const noexcept {
            return Vector{details::unitransform(details::neg<T>, contents)};
        }

        constexpr bool operator==(const Vector &t) const noexcept {
            return details::equals(contents, t.contents);
        }

        constexpr bool operator!=(const Vector &rhs) const noexcept {
            return !(rhs == *this);
        }

        constexpr T dot_product(const Vector &t) const noexcept {
            return details::Reducer<BiFunction, BiFunction, T, T, N, N>::result(details::mult<T>, details::sum<T>, 0,
                    contents, t.contents);
        }

        constexpr T magnitude() const noexcept {
            return sqrtd(dot_product(*this));
        }

        constexpr Vector normalize() const noexcept {
            return Vector{details::unitransform([this](T t) { return t / magnitude(); }, contents)};
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

        constexpr vector_type getContents() const noexcept {
            return contents;
        }
    };
}