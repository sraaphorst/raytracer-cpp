/**
 * world.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <algorithm>
#include <iterator>
#include <memory>
#include <optional>
#include <vector>

#include "pointlight.h"
#include "shape.h"
#include "vector.h"

namespace raytracer {
    class Intersection;
    class Hit;

    class World final {
        std::optional<PointLight> light;
        std::vector<std::shared_ptr<const Shape>> shapes;

    public:
        World() noexcept = default;
        World(const World&) noexcept = default;
        World(World&&) noexcept = default;

        template<typename L, typename S>
        World(const L &light, const std::vector<S> &shapes) noexcept : light{light}, shapes{shapes} {}

        World &operator=(const World &other) noexcept = default;

        bool operator==(const World &other) const noexcept;
        bool operator!=(const World &other) const noexcept;

        const std::optional<PointLight> getLightSource() const noexcept;
        const std::vector<std::shared_ptr<const Shape>> getObjects() const noexcept;

        const bool contains(const Shape &s) const noexcept;

        const std::optional<const Colour> shade_hit(const std::optional<const Hit>&) const noexcept;

        const std::vector<Intersection> intersect(const Ray &ray) const noexcept;

        static World getDefaultWorld() noexcept;
    };
}


