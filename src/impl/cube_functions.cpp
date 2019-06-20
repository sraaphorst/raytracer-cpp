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
    std::pair<double, double> checkAxis(double origin, double direction, double min_extent, double max_extent) {
        const auto tmin_numerator = min_extent - origin;
        const auto tmax_numerator = max_extent - origin;

        // If the direction is close to 0, assume parallel, in which case, no intersection.
        if (ALMOST_EQUALS(direction, 0))
            return {tmin_numerator * math_constants::inf<>,
                    tmax_numerator * math_constants::inf<>};

        auto tmin = tmin_numerator / direction;
        auto tmax = tmax_numerator / direction;
        if (tmin > tmax)
            std::swap(tmin, tmax);
        return {tmin, tmax};
    }
}