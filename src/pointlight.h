/**
 * pointlight.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

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


        constexpr Tuple getPosition() const noexcept {
            return position;
        }

        constexpr Colour getIntensity() const noexcept {
            return intensity;
        }
    };
}