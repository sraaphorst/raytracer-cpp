/**
 * intersection.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <optional>
#include <vector>

#include "hit.h"
#include "intersection.h"
#include "ray.h"

namespace raytracer {
    const std::optional<const Intersection> Intersection::hit(const std::vector<Intersection> &ints) noexcept {
        if (ints.empty())
            return {};

        Intersection const *curr = nullptr;
        for (const Intersection &i: ints) {
            if (i.getT() > 0 && (curr == nullptr || i.getT() < curr->getT()))
                curr = &i;
        }
        if (curr == nullptr || curr->getT() < 0)
            return {};
        else
            return {*curr};
    }

    std::vector<Intersection> Intersection::aggregate(std::initializer_list<Intersection> lst) noexcept {
        return std::vector<Intersection>{lst};
    }

    const std::optional<const Hit> Intersection::prepareHit(const std::optional<const Intersection> &hit,
                                                            const Ray &ray) noexcept {
        if (!hit.has_value())
            return {};
        return prepareHit(hit.value(), ray);
    }

    const Hit Intersection::prepareHit(const Intersection &hit, const Ray &ray) noexcept {
        const auto point = ray.position(hit.getT());
        const auto eyev  = -ray.getDirection();
        const auto normalv = hit.getObject().normalAt(point);
        const bool inside = normalv.dot_product(eyev) < 0;

        // Slightly offset the point from the expected value to prevent acne.
        // 1e-4 is arbitrary and depends on the scale of the scene; it is sufficient unless dealing with very
        // small distances.
        const auto adjusted_point = point + normalv * 1e-4;

        return Hit{hit, adjusted_point, eyev, (inside ? -1 : 1) * normalv, inside};
    }
}