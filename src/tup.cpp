/**
 * tup.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cmath>
#include <initializer_list>
#include <stdexcept>
#include <sstream>
#include <string>
#include <tuple>

#include "common.h"
#include "tup.h"

namespace raytracer {
    tup::tup(const tup_contents &contents): contents{contents} { check(); }
    tup::tup(tup_contents&& contents): contents{std::move(contents)} { check(); }
    tup::tup(const tup &other): contents{other.contents} { check(); }
    tup::tup(tup&& other): contents{std::move(other.contents)} { check(); }

    namespace {
        /**
         * This is kind of horrible, but initializer_lists are limited with respect to a const array, so we need
         * to work around it by implementing indexing.
         */
        template<class T>
        struct init_list_with_square_brackets {
            const std::initializer_list<T> &lst;

            init_list_with_square_brackets(const std::initializer_list<T> &lst) : lst(lst) {}

            T operator[](unsigned int index) {
                if (index < 0 || index >= lst.size()) {
                    std::stringstream str;
                    str << "tup cannot be created with initializer list of size " << lst.size() << std::endl;
                    throw std::invalid_argument(str.str());
                }
                return *(lst.begin() + index);
            }
        };

        // a function, with the short name _ (underscore) for creating
        // the _init_list_with_square_brackets out of a "regular" std::initializer_list
        template<class T>
        init_list_with_square_brackets<T> idx(const std::initializer_list<T> &list) {
            return init_list_with_square_brackets<T>(list);
        }
    };

    tup::tup(std::initializer_list<double> lst): contents{idx(lst)[0], idx(lst)[1], idx(lst)[2], idx(lst)[3]} {
        if (lst.size() != 4) {
            std::stringstream str;
            str << "tup cannot be created with initializer list of size " << lst.size() << std::endl;
            throw std::invalid_argument(str.str());
        }
        check();
    }

    void tup::check() const {
        if (contents[3] != 0 && contents[3] != 1) {
            std::stringstream str;
            str << "fourth coordinate of tup must be 0 (point) or 1 (vector): " << contents[3] << std::endl;
            throw std::invalid_argument(str.str());
        }
    }

    void tup::check_vector(const std::string op) const {
        if (contents[3] != vector_flag) {
            std::stringstream str;
            str << "tup operation " << op << " requires vectors and not points";
            throw std::invalid_argument(str.str());
        }
    }

    const tup tup::operator+(const tup &other) const {
        return tup{contents[x] + other[x], contents[y] + other[y], contents[z] + other[z], contents[w] + other[w]};
    }

    const tup tup::operator-(const tup &other) const {
        return tup{contents[x] - other[x], contents[y] - other[y], contents[z] - other[z], contents[w] - other[w]};
    }

    const tup tup::operator*(double factor) const {
        return tup{factor * contents[x], factor * contents[y], factor * contents[z], contents[w]};
    }

    const tup tup::operator/(double denominator) const {
        if (denominator == 0)
            throw std::invalid_argument("tup division by 0");
        return tup{contents[x] / denominator, contents[y] / denominator, contents[z] / denominator, contents[w]};
    }
    const tup tup::operator-() const {
        return tup{-contents[x], -contents[y], -contents[z], contents[w]};
    }

    const double tup::operator*(const tup &other) const {
        check_vector("*");
        other.check_vector("*");
        return contents[x] * other[x] + contents[y] * other[y] + contents[z] * other[z];
    }

    const tup tup::operator%(const tup &other) const {
        check_vector("%");
        other.check_vector("%");
        return tup{
            contents[y] * other[z] - contents[z] * other[y],
            contents[z] * other[x] - contents[x] * other[z],
            contents[x] * other[y] - contents[y] * other[x],
            vector_flag
        };
    }

    bool tup::operator==(const tup &rhs) const {
        for (int i=0; i < 3; ++i)
            if (!ALMOST_EQUALS(contents[i], rhs[i]))
                return false;
        return contents[3] == rhs[3];
    }

    bool tup::operator!=(const tup &rhs) const {
        return !(rhs == *this);
    }

    double tup::operator[](int pos) const {
        if (pos < 0 || pos >= 4) {
            std::stringstream str;
            str << "Invalid index into tuple: " << pos << std::endl;
            throw std::invalid_argument(str.str());
        }
        return contents[pos];
    }

    tup tup::point(double dx, double dy, double dz) {
        return tup{dx, dy, dz, point_flag};
    }

    tup tup::vector(double dx, double dy, double dz) {
        return tup{dx, dy, dz, vector_flag};
    }

    const double tup::point_flag = 1;
    const double tup::vector_flag = 0;
    const tup tup::zero_point{0, 0, 0, tup::point_flag};
    const tup tup::zero_vector{0, 0, 0, tup::vector_flag};
    const int tup::x = 0;
    const int tup::y = 1;
    const int tup::z = 2;
    const int tup::w = 3;

    std::ostream &operator<<(std::ostream &os, const tup &tup) {

        return os << (tup.isPoint() ? "point" : "vector") <<
            '(' << tup[0] << ", " << tup[1] << ", " << tup[2] << ", " << ')';
    }

    tup operator*(double factor, const tup &t) {
        return t * factor;
    }
}