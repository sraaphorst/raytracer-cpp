/**
 * cube.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include "constmath.h"
#include "cube.h"
#include "intersection.h"
#include "ray.h"
#include "shape.h"
#include "vector.h"

namespace raytracer {
    const std::vector<Intersection> Cube::localIntersection(const Ray &ray) const noexcept {
        std::vector<double> mins;
        std::vector<double> maxs;
        std::vector<Intersection> xs;

        for (size_t i = 0; i < 3; ++i) {
            const auto opt_vals = checkAxis(ray.getOrigin()[i], ray.getDirection()[i]);
            if (opt_vals) {
                const auto [vmin, vmax] = *opt_vals;
                mins.emplace_back(vmin);
                maxs.emplace_back(vmax);
            }
        }

        // If there are values, return the max of the mins and the min of the maxes, provided they actually intersect.
        if (!(mins.empty() || maxs.empty())) {
            const auto maxmin = *std::max_element(std::cbegin(mins), std::cend(mins));
            const auto minmax = *std::min_element(std::cbegin(maxs), std::cend(maxs));
            if (maxmin <= minmax) {
                // We need a shared pointer to this or we can't use shared_from_this.
                const auto ptr = std::make_shared<const Cube>(*this);
                xs.emplace_back(Intersection{maxmin, ptr});
                xs.emplace_back(Intersection{minmax, ptr});
            }
        }

        return xs;
    }

    const Tuple Cube::localNormalAt(const Tuple&) const noexcept {
        return predefined_tuples::zero_vector;
    }

    std::optional<std::pair<double, double>> Cube::checkAxis(const double origin,
            const double direction) const noexcept {
        // If the direction is close to 0, assume parallel, in which case, no intersection.
        if (ALMOST_EQUALS(direction, 0))
            return {};

        auto tmin = (-1 - origin) / direction;
        auto tmax = ( 1 - origin) / direction;
        if (tmin > tmax)
            std::swap(tmin, tmax);
        return {{tmin, tmax}};
    }
}