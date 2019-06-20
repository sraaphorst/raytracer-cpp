/**
 * cube_functions.cpp
 *
 * By Sebastian Raaphorst, 2019.
 */

#include <tuple>

#include "common.h"
#include "constmath.h"
#include "cube_functions.h"

namespace raytracer::impl {
    std::pair<double, double> checkAxis(double origin, double direction) {
        // If the direction is close to 0, assume parallel, in which case, no intersection.
        if (ALMOST_EQUALS(direction, 0))
            return {(-1 - origin) * math_constants::inf<>,
                    (1 - origin) * math_constants::inf<>};

        auto tmin = (-1 - origin) / direction;
        auto tmax = ( 1 - origin) / direction;
        if (tmin > tmax)
            std::swap(tmin, tmax);
        return {tmin, tmax};
    }
}