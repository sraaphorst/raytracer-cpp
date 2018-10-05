/**
 * material.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "vector.h"

namespace raytracer {
    class Material final {
    private:
        const Colour colour;
        const double ambient;
        const double diffuse;
        const double specular;
        const double shininess;

    public:
        constexpr Material(const Colour colour,
                           double ambient,
                           double diffuse,
                           double specular,
                           double shininess) noexcept:
            colour{colour}, ambient{ambient}, diffuse{diffuse}, specular{specular}, shininess{shininess} {}

        constexpr Material() noexcept:
            Material{DEFAULT_COLOUR, DEFAULT_AMBIENT, DEFAULT_DIFFUSE, DEFAULT_SPECULAR, DEFAULT_SHININESS} {}

        constexpr Material(const Material&) = default;
        constexpr Material(Material&&) = default;

        constexpr Colour getColour() const noexcept {
            return colour;
        }
        constexpr double getAmbient() const noexcept {
            return ambient;
        }
        constexpr double getDiffuse() const noexcept {
            return diffuse;
        }
        constexpr double getSpecular() const noexcept {
            return specular;
        }
        constexpr double getShininess() const noexcept {
            return shininess;
        }

        constexpr static Colour DEFAULT_COLOUR    = predefined_colours::white;
        constexpr static double DEFAULT_AMBIENT   = 0.1;
        constexpr static double DEFAULT_DIFFUSE   = 0.9;
        constexpr static double DEFAULT_SPECULAR  = 0.9;
        constexpr static double DEFAULT_SHININESS = 200;
    };
}