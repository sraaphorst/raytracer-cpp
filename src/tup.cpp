/**
 * tup.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>

#include "tup.h"

namespace raytracer {

    std::ostream &operator<<(std::ostream &os, const tup &tup) {
        if (tup.isPoint()) os << "point";
        else if (tup.isVector()) os << "vector";
        else os << "tup";

        os << '(' << tup[0] << ", " << tup[1] << ", " << tup[2];
        if (!(tup.isVector() || tup.isPoint())) os << ", " << tup[3];
        return os << ')';
    }

    tup operator*(double factor, const tup &t) {
        return t * factor;
    }
}