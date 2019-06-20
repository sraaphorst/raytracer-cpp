/**
 * cube_functions.h
 *
 * By Sebastian Raaphorst, 2019.
 */

#pragma once

#include <tuple>

namespace raytracer::impl {
    std::pair<double, double> checkAxis(double origin, double direction, double min_extent, double max_extent);
}



