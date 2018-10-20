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
        std::vector<std::shared_ptr<Shape>> shapes;

    public:
        World() noexcept = default;
        World(const World&) = default;
        World(World&&) noexcept = default;

        template<typename L, typename S>
        World(const L &light, const std::vector<S> &shapes) noexcept : light{light}, shapes{shapes} {}

        World &operator=(const World &other) noexcept = default;

        bool operator==(const World &other) const noexcept;
        bool operator!=(const World &other) const noexcept;

        const std::optional<PointLight> getLightSource() const noexcept;
        void setLightSource(const PointLight&) noexcept;
        void clearLightSource() noexcept;

        std::vector<std::shared_ptr<Shape>> getObjects() noexcept;
        const std::vector<std::shared_ptr<Shape>> getObjects() const noexcept;
        const bool contains(const Shape &s) const noexcept;

        const std::vector<Intersection> intersect(const Ray &ray) const noexcept;
        const std::optional<const Colour> shadeHit(const std::optional<const Hit>&,
                int remaining = MAX_RECURSIONS) const noexcept;
        const Colour colourAt(const Ray &ray, int remaining = MAX_RECURSIONS) const noexcept;

        /// Determine if a point is in shadow, i.e. there is something between this point and the light source.
        bool isShadowed(const Tuple &point) const noexcept;

        /// Get the reflected colour for a hit. Limit the recursion.
        const Colour reflectedColour(const Hit&, int remaining = MAX_RECURSIONS) const noexcept;

        static World getDefaultWorld() noexcept;

        constexpr static int MAX_RECURSIONS = 5;
    };
}


