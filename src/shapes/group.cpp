/**
 * group.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <algorithm>
#include <iterator>
#include <limits>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <vector>

#include "bounding_box.h"
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

    void Group::clearShapes() noexcept {
        for (auto &shape: shapes)
            shape->setParent(nullptr);
        shapes.clear();
    }

    BoundingBox Group::bounds() const {
        BoundingBox box{};
        for (const auto &shape: shapes)
            box.addBox(shape->parentSpaceBounds());
        return box;
    }

    const std::vector<Intersection> Group::localIntersection(const Ray &ray) const noexcept {
        std::vector<Intersection> xs{};
        for (auto &shape: shapes) {
            auto xss = shape->intersect(ray);
            std::copy(xss.begin(), xss.end(), std::back_inserter(xs));
        }

        // Now sort by t.
        std::sort(xs.begin(), xs.end(), [](const auto &x1, const auto &x2) { return x1.getT() < x2.getT(); });
        return xs;
    }

    const Tuple Group::localNormalAt(const Tuple&) const {
        throw std::domain_error("Group::localNormalAt cannot be called");
    }
}
