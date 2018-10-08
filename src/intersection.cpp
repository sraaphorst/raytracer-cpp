/**
 * intersection.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <optional>

#include "intersection.h"
#include "ray.h"

namespace raytracer {
    const std::optional<const Intersection> Intersection::hit(const std::vector<Intersection> &ints) {
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

    static const std::optional<const Intersection> Intersection::prepare_hit(const std::optional<const Intersection> &hit,
                                                               const Ray &ray) {
        if (!hit.has_value())
            return {};

        const auto point = ray.position(hit->getT());
        const auto eyev  = ray.getDirection();
        const auto normalv = hit->getObject().normalAt(point);

    }
}