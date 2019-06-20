/**
 * cube.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <limits>
#include <memory>
#include <tuple>
#include <vector>

#include "bounding_box.h"
#include "constmath.h"
#include "cube.h"
#include "cube_functions.h"
#include "intersection.h"
#include "ray.h"
#include "shape.h"
#include "vec.h"

using namespace raytracer::impl;

namespace raytracer::shapes {
    Cube::Cube(dummy d) noexcept: Shape{d} {}

    std::shared_ptr<Cube> Cube::createCube() noexcept {
        auto cube = std::make_shared<Cube>(dummy{});
        registerInstance(cube);
        return cube;
    }


    BoundingBox Cube::bounds() const {
        return BoundingBox{make_point(-1, -1, -1), make_point(1, 1, 1)};
    }

    const std::vector<Intersection> Cube::localIntersection(const Ray &ray) const noexcept {
        std::vector<double> mins;
        std::vector<double> maxs;
        std::vector<Intersection> xs;

        for (size_t i = 0; i < 3; ++i) {
            const auto [vmin, vmax] = checkAxis(ray.getOrigin()[i], ray.getDirection()[i]);
            mins.emplace_back(vmin);
            maxs.emplace_back(vmax);
        }

        const auto tmin = *std::max_element(std::cbegin(mins), std::cend(mins));
        const auto tmax = *std::min_element(std::cbegin(maxs), std::cend(maxs));
        if (tmin <= tmax) {
            xs.emplace_back(Intersection{tmin, shared_from_this()});
            xs.emplace_back(Intersection{tmax, shared_from_this()});
        }

        return xs;
    }

    const Tuple Cube::localNormalAt(const Tuple &point) const noexcept {
        const auto maxc = std::max(const_absd(point[0]), std::max(const_absd(point[1]), const_absd(point[2])));
        if (maxc == const_absd(point[0]))
            return make_vector(point[0], 0, 0);
        else if (maxc == const_absd(point[1]))
            return make_vector(0, point[1], 0);
        else
            return make_vector(0, 0, point[2]);
    }
}