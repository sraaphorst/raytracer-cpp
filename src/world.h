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
#include "shapes/shape.h"
#include "vec.h"

namespace raytracer {
    namespace impl {
        class Intersection;
        class Hit;
        class Ray;
    }

    class World final {
        std::optional<PointLight> light;
        std::vector<std::shared_ptr<shapes::Shape>> shapes;

    public:
        World() noexcept = default;
        World(const World&) = default;
        World(World&&) noexcept = default;

        template<typename L, typename S>
        World(const L &light, const std::vector<S> &shapes) noexcept : light{light}, shapes{shapes} {}

        World &operator=(const World &other) = default;

        bool operator==(const World &other) const noexcept;
        bool operator!=(const World &other) const noexcept;

        const std::optional<PointLight> getLightSource() const noexcept;
        void setLightSource(const PointLight&) noexcept;
        void clearLightSource() noexcept;

        std::vector<std::shared_ptr<shapes::Shape>> &getObjects() noexcept;
        const std::vector<std::shared_ptr<shapes::Shape>> &getObjects() const noexcept;

        const bool contains(const std::shared_ptr<shapes::Shape> &sptr) const noexcept;
        const bool contains(const shapes::Shape &s) const noexcept;

        const std::vector<impl::Intersection> intersect(const impl::Ray &ray) const noexcept;
        const std::optional<const Colour> shadeHit(const std::optional<const impl::Hit>&,
                int remaining = MAX_RECURSIONS) const noexcept;
        const Colour colourAt(const impl::Ray &ray, int remaining = MAX_RECURSIONS) const noexcept;

        /// Determine if a point is in shadow, i.e. there is something between this point and the light source.
        bool isShadowed(const Tuple &point) const noexcept;

        /// Get the reflected colour for a hit. Limit the recursion.
        const Colour reflectedColour(const impl::Hit&, int remaining = MAX_RECURSIONS) const noexcept;

        /// Get the refracted colour for a hit. Limit the recursion.
        const Colour refractedColour(const impl::Hit&, int remaining = MAX_RECURSIONS) const noexcept;

        static World getDefaultWorld() noexcept;

        constexpr static int MAX_RECURSIONS = 5;
    };
}
