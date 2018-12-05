/**
 * world.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <climits>
#include <memory>
#include <mutex>
#include <vector>

#include "hit.h"
#include "intersection.h"
#include "ray.h"
#include "shape.h"
#include "sphere.h"
#include "world.h"

using namespace raytracer::impl;
using namespace raytracer::shapes;

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

    std::vector<std::shared_ptr<Shape>> &World::getObjects() noexcept {
        return shapes;
    }

    const std::vector<std::shared_ptr<Shape>> &World::getObjects() const noexcept {
        return shapes;
    }

    bool World::contains(const std::shared_ptr<Shape> &sptr) const noexcept {
        return contains(*sptr);
    }

    bool World::contains(const Shape &s) const noexcept {
        for (const auto i: shapes)
            if (*i == s)
                return true;
        return false;
    }

    const std::vector<Intersection> World::intersect(const Ray &ray) const noexcept {
        std::mutex intersection_mutex;
        std::vector<Intersection> intersections;

        // Unless we have a lot of shapes here, this is unlikely to have a lot of benefit.
#pragma omp parallel for shared(intersections, ray)
        for (size_t i = 0; i < shapes.size(); ++i) {
            const auto &s = shapes[i];
            const std::vector<Intersection> vi = s->intersect(ray);

            std::lock_guard<std::mutex> guard(intersection_mutex);
            intersections.insert(std::end(intersections), std::cbegin(vi), std::cend(vi));
        }

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
        const auto &material = hit->getObject()->getMaterial();
        const auto surface = material.lighting(light.value(), hit->getObject(),
                hit->getPoint(), hit->getEyeVector(), hit->getNormalVector(), shadowed);
        const auto reflected = reflectedColour(*hit, remaining);
        const auto refracted = refractedColour(*hit, remaining);

        if (material.getReflectivity() > 0 && material.getTransparency() > 0) {
            const auto reflectance = hit->schlick();
            return surface + reflected * reflectance + refracted * (1 - reflectance);
        }
        return surface + reflected + refracted;
    }

    const Colour World::colourAt(const Ray &ray, int remaining) const noexcept {
        const auto xs = intersect(ray);
        const auto hit = Intersection::hit(xs);
        if (!hit.has_value())
            return predefined_colours::black;

        const auto populated_hit = Intersection::prepareHit(hit, ray, xs);
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

        const auto reflectivity = hit.getObject()->getMaterial().getReflectivity();
        if (reflectivity == 0)
            return predefined_colours::black;
        const Ray reflect_ray{hit.getPoint(), hit.getReflectVector()};
        const auto colour = colourAt(reflect_ray, remaining - 1);
        return colour * reflectivity;
    }

    const Colour World::refractedColour(const Hit &hit, int remaining) const noexcept {
        if (remaining < 1)
            return predefined_colours::black;

        const auto transparency = hit.getObject()->getMaterial().getTransparency();
        if (transparency == 0)
            return predefined_colours::black;

        // Apply Snell's Law to find the angle of refraction.
        const auto n_ratio = hit.getN1() / hit.getN2();
        const auto cos_i = hit.getEyeVector().dot_product(hit.getNormalVector());
        const auto sin2_t = n_ratio * n_ratio * (1 - cos_i * cos_i);

        // If sin2_t > 1, we have total internal reflection.
        if (sin2_t > 1)
            return predefined_colours::black;

        // Create the refracted ray.
        const auto cos_t = sqrtd(1 - sin2_t);
        const auto direction = hit.getNormalVector() * (n_ratio * cos_i - cos_t) - hit.getEyeVector() * n_ratio;
        const Ray refract_ray{hit.getUnderPoint(), direction};

        // Find its colour.
        const auto colour = colourAt(refract_ray, remaining - 1) * transparency;
        return colour;
    }

    World World::getDefaultWorld() noexcept {
        auto s1 = Sphere::createSphere();
        s1->setMaterial(Material{make_colour(0.8, 1.0, 0.6), Material::DEFAULT_AMBIENT, 0.7, 0.2, Material::DEFAULT_SHININESS});

        auto s2 = Sphere::createSphere();
        s2->setTransformation(scale(0.5, 0.5, 0.5));

        std::vector<std::shared_ptr<Shape>> v = {s1, s2};
        return World{PointLight{make_point(-10, 10, -10), predefined_colours::white}, v};
    }
}