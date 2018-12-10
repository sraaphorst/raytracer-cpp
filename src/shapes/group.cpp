/**
 * group.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <limits>
#include <memory>
#include <tuple>
#include <vector>

#include "constmath.h"
#include "group.h"
#include "intersection.h"
#include "ray.h"
#include "shape.h"
#include "vec.h"

using namespace raytracer;
using namespace raytracer::impl;

namespace raytracer::shapes {
    Group::Group(dummy d) noexcept: Shape{d} {}

    std::shared_ptr<Group> Group::createGroup() noexcept {
        auto group = std::make_shared<Group>(dummy{});
        registerInstance(group);
        return group;
    }

    const std::vector<std::shared_ptr<Shape>> Group::getShapes() const noexcept {
        return shapes;
    }

    const std::vector<Intersection> Group::localIntersection(const Ray&) const noexcept {
        return {};
    }

    const Tuple Group::localNormalAt(const Tuple&) const noexcept {
        return predefined_tuples::zero_vector;
    }
}
