/**
 * tup.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <array>
#include <initializer_list>
#include <ostream>
#include <string>
#include <tuple>

#include "common.h"
#include "init_list.h"
#include "constmath.h"

namespace raytracer {
    using tup_contents = std::array<double, 4>;

    class tup {
    private:
        const tup_contents contents;

        constexpr void check_vector() const {
            if (contents[w] != vector_flag)
                throw std::invalid_argument("tup operation requires vectors");
        }

    public:
        constexpr explicit tup(const tup_contents &contents): contents{contents} {};
        constexpr explicit tup(tup_contents&& contents): contents{contents} {};
        constexpr tup(const tup &other) = default;
        constexpr tup(tup&& other) = default;
        constexpr tup(const std::initializer_list<double> lst): contents{idx(lst)[x], idx(lst)[y], idx(lst)[z], idx(lst)[w]} {}

        template<size_t N>
        constexpr explicit tup(const double (&arr)[N]): contents{arr} {
            static_assert(N == 4, "tup must be initialized with four values");
        };

        ~tup() = default;

        constexpr inline bool isPoint() const noexcept { return contents[w] == point_flag; };
        constexpr inline bool isVector() const noexcept { return contents[w] == vector_flag; };

        constexpr tup operator+(const tup &other) const noexcept {
            return tup({contents[x] + other[x], contents[y] + other[y], contents[z] + other[z], contents[w] + other[w]});
        }

        constexpr tup operator-(const tup &other) const noexcept {
            return tup({contents[x] - other[x], contents[y] - other[y], contents[z] - other[z], contents[w] - other[w]});
        }

        constexpr tup operator*(double factor) const noexcept {
            return tup({factor * contents[x], factor * contents[y], factor * contents[z], factor * contents[w]});
        }

        constexpr tup operator/(double denominator) const noexcept {
            return tup({contents[x] / denominator, contents[y] / denominator, contents[z] / denominator, contents[w] / denominator});
        }

        constexpr tup operator-() const noexcept {
            return tup({-contents[x], -contents[y], -contents[z], -contents[w]});
        }

        /// Dot product
        constexpr double dot_product(const tup &other) const noexcept {
            return contents[x] * other[x] + contents[y] * other[y] + contents[z] * other[z] + contents[w] * other[w];
        }

        /// Cross product
        constexpr tup cross_product(const tup &other) const {
            check_vector();
            other.check_vector();
            return tup({
                contents[y] * other[z] - contents[z] * other[y],
                contents[z] * other[x] - contents[x] * other[z],
                contents[x] * other[y] - contents[y] * other[x],
                vector_flag
            });
        }

        /// Hadamard product
        constexpr tup hadamard_product(const tup &other) const {
            check_vector();
            other.check_vector();
            return tup({
               contents[x] * other[x],
               contents[y] * other[y],
               contents[z] * other[z],
               contents[w] * other[w]
            });
        }

        constexpr bool operator==(const tup &rhs) const noexcept {
            return ALMOST_EQUALS(contents[x], rhs[x]) &&
                   ALMOST_EQUALS(contents[y], rhs[y]) &&
                   ALMOST_EQUALS(contents[z], rhs[z]) &&
                   ALMOST_EQUALS(contents[w], rhs[w]);
        }

        constexpr bool operator!=(const tup &rhs) const noexcept {
            return !(rhs == *this);
        }

        constexpr double operator[](size_t pos) const noexcept {
            return contents[pos];
        }

        /// Magnitude of the tuple.
        constexpr double magnitude() const noexcept {
            return sqrtd(
                    contents[x] * contents[x] +
                    contents[y] * contents[y] +
                    contents[z] * contents[z] +
                    contents[w] * contents[w]
                    );
        }

        /// Normalization of the tuple.
        constexpr tup normalize() const noexcept {
            return *this / magnitude();
        }

        /// Output as a tuple to get all vals at once.
        constexpr std::tuple<const double&, const double&, const double&, const double&> asTuple() const {
            return std::tie(contents[x], contents[y], contents[z], contents[w]);
        }

        /// Factory methods.
        constexpr static tup point(double dx, double dy, double dz) noexcept {
            return tup({dx, dy, dz, point_flag});
        }

        constexpr static tup vector(double dx, double dy, double dz) noexcept {
            return tup({dx, dy, dz, vector_flag});
        }

        friend std::ostream &operator<<(std::ostream &os, const tup &tup);
        friend tup operator*(double factor, const tup &t);

        constexpr static double point_flag = 1;
        constexpr static double vector_flag = 0;
        constexpr static int x = 0;
        constexpr static int y = 1;
        constexpr static int z = 2;
        constexpr static int w = 3;
    };

    struct tup_constants {
        static constexpr tup zero_point{{0, 0, 0, tup::point_flag}};
        static constexpr tup zero_vector{{0, 0, 0, tup::vector_flag}};
        static constexpr tup x1{{1, 0, 0, tup::vector_flag}};
        static constexpr tup y1{{0, 1, 0, tup::vector_flag}};
        static constexpr tup z1{{0, 0, 1, tup::vector_flag}};
    };
}



