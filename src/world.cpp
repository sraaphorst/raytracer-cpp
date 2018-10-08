/**
 * world.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <memory>
#include <vector>

#include "intersection.h"
#include "ray.h"
#include "shape.h"
#include "sphere.h"
#include "world.h"

namespace raytracer {
    bool World::operator==(const World &other) const noexcept {
        return light == other.light && shapes == other.shapes;
    }

    bool World::operator!=(const World &other) const noexcept {
        return !(*this == other);
    }

    const std::optional<PointLight> World::getLightSource() const noexcept {
        return light;
    }

    const std::vector<std::shared_ptr<const Shape>> World::getObjects() const noexcept {
        return shapes;
    }

    const std::vector<Intersection> World::intersect(const Ray &ray) const noexcept {
        std::vector<Intersection> intersections;
        std::for_each(std::cbegin(shapes), std::cend(shapes), [&ray, &intersections](const auto &s) {
            const std::vector<Intersection> vi = s->intersect(ray);
            intersections.insert(std::end(intersections), std::cbegin(vi), std::cend(vi));
        });

        std::sort(std::begin(intersections), std::end(intersections), [](const auto i1, const auto i2) {
            return i1.getT() < i2.getT();
        });

        return intersections;
    }

    World World::getDefaultWorld() noexcept {
        std::shared_ptr<const Shape> s1{new Sphere{notransform(),
            Material{make_colour(0.8, 1.0, 0.6), Material::DEFAULT_AMBIENT, 0.7, 0.2, Material::DEFAULT_SHININESS}
        }};
        std::shared_ptr<const Shape> s2{new Sphere{scale(0.5, 0.5, 0.5)}};
        std::vector<std::shared_ptr<const Shape>> v = {s1, s2};
        return World{PointLight{make_point(-10, 10, -10), predefined_colours::white}, v};
    }
}