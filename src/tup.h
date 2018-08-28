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

namespace raytracer {
    using tup_contents = std::array<double, 4>;

    class tup final {
    private:
        const tup_contents contents;

        void check() const;
        void check_vector(std::string op) const;

    public:
        tup(const tup_contents &contents);
        tup(tup_contents&& contents);
        tup(const tup &other);
        tup(tup&& other);

        /// This creates a bunch of intermediate objects and thus is not very efficient.
        tup(std::initializer_list<double> lst);

        inline bool isPoint() const { return contents[w] == 1; };
        inline bool isVector() const { return contents[w] == 0; };

        const tup operator+(const tup &other) const;
        const tup operator-(const tup &other) const;
        const tup operator*(double factor) const;
        const tup operator/(double denominator) const;
        const tup operator-() const;

        /// Dot product
        const double operator*(const tup &other) const;

        /// Cross product
        const tup operator%(const tup &other) const;

        bool operator==(const tup &rhs) const;
        bool operator!=(const tup &rhs) const;
        double operator[](int pos) const;

        friend std::ostream &operator<<(std::ostream &os, const tup &tup);
        friend tup operator*(double factor, const tup &t);

        static const double point_flag;
        static const double vector_flag;
        static const tup zero_point;
        static const tup zero_vector;
        static const int x, y, z, w;

        /// Factory methods.
        static tup point(double dx, double dy, double dz);
        static tup vector(double dx, double dy, double dz);
    };
}



