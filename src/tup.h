/**
 * tup.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <initializer_list>
#include <array>
#include <ostream>

namespace raytracer {
    using tup_contents = std::array<double, 4>;

    class tup final {
    private:
        const tup_contents contents;

        void check() const;
        void check_vector() const;

    public:
        tup(const tup_contents &contents);
        tup(tup_contents&& contents);
        tup(const tup &other);
        tup(tup&& other);

        /// This creates a bunch of intermediate objects and thus is not very efficient.
        tup(std::initializer_list<double> lst);

        const tup operator+(const tup &other) const;
        const tup operator-(const tup &other) const;
        const double operator*(const tup &other) const;
        const tup operator-() const;

        /// Dot product
        const tup operator*(double factor) const;

        /// Cross product
        const tup operator%(const tup &other) const;

        bool operator==(const tup &rhs) const;
        bool operator!=(const tup &rhs) const;
        double operator[](int pos) const;

        friend std::ostream &operator<<(std::ostream &os, const tup &tup);
        friend tup operator*(double factor, const tup &t);

        static tup zero_point;
        static tup zero_vector;
        static int x, y, z, w;
    };
}



