/**
 * bounding_box.cpp
 *
 * By Sebastian Raaphorst, 2019.
 */

#include "affine_transform.h"
#include "cube_functions.h"
#include "ray.h"
#include "vec.h"
#include "bounding_box.h"

namespace raytracer::impl {
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

    void BoundingBox::addBox(const BoundingBox &box) noexcept {
        addPoint(box.getMinPoint());
        addPoint(box.getMaxPoint());
    }

    bool BoundingBox::containsPoint(const Tuple &point) const noexcept {
        const auto x = point[tuple_constants::x];
        const auto y = point[tuple_constants::y];
        const auto z = point[tuple_constants::z];
        return
            min_point[tuple_constants::x] <= x && x <= max_point[tuple_constants::x] &&
            min_point[tuple_constants::y] <= y && y <= max_point[tuple_constants::y] &&
            min_point[tuple_constants::z] <= z && z <= max_point[tuple_constants::z];

    }

    bool BoundingBox::containsBox(const BoundingBox &other) const noexcept {
        return containsPoint(other.getMinPoint()) && containsPoint(other.getMaxPoint());
    }

    BoundingBox BoundingBox::transform(const Transformation &trans) const noexcept {
        BoundingBox box{};
        box.addPoint(trans * min_point);
        box.addPoint(trans * make_point(min_point[tuple_constants::x], min_point[tuple_constants::y], max_point[tuple_constants::z]));
        box.addPoint(trans * make_point(min_point[tuple_constants::x], max_point[tuple_constants::y], min_point[tuple_constants::z]));
        box.addPoint(trans * make_point(min_point[tuple_constants::x], max_point[tuple_constants::y], max_point[tuple_constants::z]));
        box.addPoint(trans * make_point(max_point[tuple_constants::x], min_point[tuple_constants::y], min_point[tuple_constants::z]));
        box.addPoint(trans * make_point(max_point[tuple_constants::x], min_point[tuple_constants::y], max_point[tuple_constants::z]));
        box.addPoint(trans * make_point(max_point[tuple_constants::x], max_point[tuple_constants::y], min_point[tuple_constants::z]));
        box.addPoint(trans * max_point);

        return box;
    }

    bool BoundingBox::intersects(const Ray &ray) const noexcept {
        std::vector<double> mins;
        std::vector<double> maxs;

        for (size_t i = 0; i < 3; ++i) {
            const auto [vmin, vmax] = checkAxis(ray.getOrigin()[i], ray.getDirection()[i]);
            mins.emplace_back(vmin);
            maxs.emplace_back(vmax);
        }

        const auto tmin = *std::max_element(std::cbegin(mins), std::cend(mins));
        const auto tmax = *std::min_element(std::cbegin(maxs), std::cend(maxs));
        return tmin <= tmax;
    }
}