/**
 * world.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <climits>
#include <memory>
#include <vector>

#include "hit.h"
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

    void World::setLightSource(const PointLight &pl) noexcept {
        light = pl;
    }

    void World::clearLightSource() noexcept {
        light = {};
    }

    std::vector<std::shared_ptr<Shape>> World::getObjects() noexcept {
        return shapes;
    }

    const std::vector<std::shared_ptr<Shape>> World::getObjects() const noexcept {
        return shapes;
    }

    const bool World::contains(const Shape &s) const noexcept {
        for (const auto i: shapes)
            if (*i == s)
                return true;
        return false;
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

    const std::optional<const Colour> World::shadeHit(const std::optional<const Hit> &hit,
            int remaining) const noexcept {
        if (!(light.has_value() && hit.has_value()))
            return {};

        const auto shadowed = isShadowed(hit->getPoint());
        const auto surface = hit->getObject().getMaterial().lighting(light.value(), hit->getObject(),
                hit->getPoint(), hit->getEyeVector(), hit->getNormalVector(), shadowed);
        const auto reflected = reflectedColour(*hit, remaining);
        return surface + reflected;
    }

    const Colour World::colourAt(const Ray &ray, int remaining) const noexcept {
        const auto xs = intersect(ray);
        const auto hit = Intersection::hit(xs);
        if (!hit.has_value())
            return predefined_colours::black;

        const auto populated_hit = Intersection::prepareHit(hit, ray);
        const auto shade = shadeHit(populated_hit, remaining);
        return shade.value_or(predefined_colours::black);
    }

    bool World::isShadowed(const Tuple &point) const noexcept {
        if (!light.has_value())
            return false;
        const auto v = light.value().getPosition() - point;
        const auto distance = v.magnitude();
        const auto direction = v.normalize();

        // Cast a shadow ray to see if it intersects anything.
        const Ray ray{point, direction};
        const auto xs = intersect(ray);
        const auto hit = Intersection::hit(xs);
        return hit.has_value() && hit.value().getT() < distance;
    }

    const Colour World::reflectedColour(const Hit &hit, int remaining) const noexcept {
        if (remaining < 1)
            return predefined_colours::black;

        const auto reflectivity = hit.getObject().getMaterial().getReflectivity();
        if (reflectivity == 0)
            return predefined_colours::black;
        const Ray reflect_ray{hit.getPoint(), hit.getReflectVector()};
        const auto colour = colourAt(reflect_ray, remaining - 1);
        return colour * reflectivity;
    }

    World World::getDefaultWorld() noexcept {
        std::shared_ptr<Shape> s1{new Sphere{notransform(),
            Material{make_colour(0.8, 1.0, 0.6), Material::DEFAULT_AMBIENT, 0.7, 0.2, Material::DEFAULT_SHININESS}
        }};
        std::shared_ptr<Shape> s2{new Sphere{scale(0.5, 0.5, 0.5)}};
        std::vector<std::shared_ptr<Shape>> v = {s1, s2};
        return World{PointLight{make_point(-10, 10, -10), predefined_colours::white}, v};
    }
}