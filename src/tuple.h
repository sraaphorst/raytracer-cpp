/**
 * tuple.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <stdexcept>
#include <string>
#include <tuple>

#include "vector.h"

namespace raytracer {
    class Tuple final: public Vector<double, 4> {
    private:
        constexpr void check_vector() const {
            if ((*this)[w] != vector_flag)
                throw std::invalid_argument{"tup operation requires vectors"};
        }

    public:
        constexpr explicit Tuple(const vector_type &contents) noexcept : Vector{contents} {}
        constexpr explicit Tuple(vector_type&& contents) noexcept : Vector{contents} {}
        constexpr Tuple(const Tuple &other) noexcept = default;
        constexpr Tuple(Tuple&& other) noexcept = default;

        /// Variadic template constructor
        template<typename... V>
        constexpr Tuple(V... rs) noexcept : Vector{rs...} {}

        /// We need to be able to convert from Vector to use Vector ops.
        constexpr Tuple(const Vector<double, 4> &v): Vector{v} {}
        constexpr Tuple(Vector<double, 4>&& v): Vector{v} {}

        ~Tuple() = default;

        constexpr inline bool isPoint() const noexcept { return (*this)[w] == point_flag; }
        constexpr inline bool isVector() const noexcept { return (*this)[w] == vector_flag; }

        /// Cross product
        constexpr Tuple cross_product(const Tuple &other) const {
            check_vector();
            other.check_vector();
            return Tuple{
                (*this)[y] * other[z] - (*this)[z] * other[y],
                (*this)[z] * other[x] - (*this)[x] * other[z],
                (*this)[x] * other[y] - (*this)[y] * other[x],
                vector_flag
            };
        }

        constexpr Tuple operator+(const Tuple &other) const noexcept {
            return Tuple{((const Vector&)*this + (const Vector&)other).getContents()};
        }

        constexpr Tuple operator-(const Tuple &other) const noexcept {
            return Tuple{((const Vector&)*this - (const Vector&)other).getContents()};
        }

        constexpr Tuple operator*(const Tuple &other) const noexcept {
            return Tuple{((const Vector&)*this * (const Vector&)other).getContents()};
        }

        constexpr Tuple operator*(double factor) const noexcept {
            return Tuple{((const Vector&)*this * factor).getContents()};
        }

        constexpr Tuple operator/(const Tuple &other) const noexcept {
            return Tuple{((const Vector&)*this / (const Vector&)other).getContents()};
        }

        constexpr Tuple operator-() const noexcept {
            return Tuple{(- (const Vector&)*this).getContents()};
        }

        friend Tuple operator*(double factor, const Tuple &t) {
            return Tuple{t * factor};
        }


        /// Output as a tuple to get all vals at once.
        constexpr std::tuple<const double&, const double&, const double&, const double&> asTuple() const {
            return std::make_tuple((*this)[x], (*this)[y], (*this)[z], (*this)[w]);
        }

        /// Factory methods
        constexpr static Tuple point(double dx, double dy, double dz) {
            return Tuple{dx, dy, dz, point_flag};
        }

        constexpr static Tuple vector(double dx, double dy, double dz) {
            return Tuple{dx, dy, dz, vector_flag};
        }

        constexpr static double point_flag = 1;
        constexpr static double vector_flag = 0;
        constexpr static int x = 0;
        constexpr static int y = 1;
        constexpr static int z = 2;
        constexpr static int w = 3;
    };

    struct tuple_constants {
        static constexpr Tuple zero_point{0, 0, 0, Tuple::point_flag};
        static constexpr Tuple zero_vector{0, 0, 0, Tuple::vector_flag};
        static constexpr Tuple x1{1, 0, 0, Tuple::vector_flag};
        static constexpr Tuple y1{0, 1, 0, Tuple::vector_flag};
        static constexpr Tuple z1{0, 0, 1, Tuple::vector_flag};
    };
}
