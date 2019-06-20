/**
 * bounding_box.cpp
 *
 * By Sebastian Raaphorst, 2019.
 */

#include "vec.h"
#include "bounding_box.h"

namespace raytracer {
    void BoundingBox::addPoint(const Tuple &point) noexcept {
        const auto minx = point[tuple_constants::x] < min_point[tuple_constants::x]
                          ? point[tuple_constants::x] : min_point[tuple_constants::x];
        const auto miny = point[tuple_constants::y] < min_point[tuple_constants::y]
                          ? point[tuple_constants::y] : min_point[tuple_constants::y];
        const auto minz = point[tuple_constants::z] < min_point[tuple_constants::z]
                          ? point[tuple_constants::z] : min_point[tuple_constants::z];

        const auto maxx = point[tuple_constants::x] > max_point[tuple_constants::x]
                          ? point[tuple_constants::x] : max_point[tuple_constants::x];
        const auto maxy = point[tuple_constants::y] > max_point[tuple_constants::y]
                          ? point[tuple_constants::y] : max_point[tuple_constants::y];
        const auto maxz = point[tuple_constants::z] > max_point[tuple_constants::z]
                          ? point[tuple_constants::z] : max_point[tuple_constants::z];

        min_point = make_point(minx, miny, minz);
        max_point = make_point(maxx, maxy, maxz);
    }
}