/**
 * pointlight.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <optional>

#include "vector.h"

namespace raytracer {
    class PointLight final {
    private:
        const Tuple position;
        const Colour intensity;

    public:
        template<typename T, typename S>
        constexpr PointLight(T &&position, S &&intensity) noexcept: position{position}, intensity{intensity} {}
        constexpr PointLight(const PointLight&) noexcept = default;
        constexpr PointLight(PointLight&&) noexcept = default;

        constexpr bool operator==(const std::optional<PointLight> &other) const noexcept {
            return other.has_value() && (*this == other.value());
        }

        constexpr bool operator==(const PointLight &other) const noexcept {
            return position == other.position && intensity == other.intensity;
        }

        template<typename T>
        constexpr bool operator!=(T&& other) const noexcept {
            return !(*this == other);
        }

        constexpr Tuple getPosition() const noexcept {
            return position;
        }

        constexpr Colour getIntensity() const noexcept {
            return intensity;
        }
    };
}