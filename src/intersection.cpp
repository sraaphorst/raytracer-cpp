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
#include "shape.h"

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
                                                            const Ray &ray,
                                                            const std::vector<Intersection> &xs) noexcept {
        if (!hit.has_value())
            return {};
        return prepareHit(hit.value(), ray);
    }

    const Hit Intersection::prepareHit(const Intersection &hit,
                                       const Ray &ray,
                                       const std::vector<Intersection> &xs) noexcept {
        const auto point = ray.position(hit.getT());
        const auto eyev  = -ray.getDirection();
        const auto normalv = hit.getObject().normalAt(point);
        const auto reflectv = ray.getDirection().reflect(normalv);
        const bool inside = normalv.dot_product(eyev) < 0;

        // Slightly offset the point from the expected value to prevent acne.
        // 1e-4 is arbitrary and depends on the scale of the scene; it is sufficient unless dealing with very
        // small distances.
        const auto adjusted_point = point + normalv * 1e-4;

        // This is where the refracted rays will originate.
        // As with adjusted_point, we have to slightly offset this, but in the opposite direction.
        const auto under_point = point - normalv * 1e-4;

        double n1, n2;
        std::vector<std::reference_wrapper<const Shape>> containers;
        for (const auto &x: xs) {
            if (x == hit)
                n1 = containers.empty() ? 1 : containers.back().get().getMaterial().getRefractiveIndex();

            auto iter = std::find_if(std::begin(containers), std::end(containers),
                    [&x](const auto &c) { return c.get() == x.getObject();});
            if (iter != std::end(containers))
                containers.erase(iter);
            else
                containers.emplace_back(x.getObject());

            if (x == hit) {
                n2 = containers.empty() ? 1 : containers.back().get().getMaterial().getRefractiveIndex();
                break;
            }
        }

        return Hit{hit, adjusted_point, under_point, eyev, (inside ? -1 : 1) * normalv, reflectv, inside, n1, n2};
    }
}