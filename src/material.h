/**
 * material.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

/** TODO:constexpr If we want to make this constexpr, we'll need to implement pow as a constexpr function.
 *
 * Taylor series expansion:
 * a^x = e^{x ln a} = 1 + x ln a / 1! + (x ln a)^2 / 2! + (x ln a)^3 / 3! + ...
 *
 * Wikipedia page on calculating ln:
 * https://en.wikipedia.org/wiki/Natural_logarithm#High_precision
 *
 * Elbeno constexpr math library:
 * https://github.com/elbeno/constexpr/blob/master/src/include/cx_math.h
 */
#include <cmath>

#include "pointlight.h"
#include "vector.h"

namespace raytracer {
    class Material final {
    private:
        Colour colour;
        double ambient;
        double diffuse;
        double specular;
        double shininess;

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

        Material &operator=(const Material&) = default;
        Material &operator=(Material&&) = default;

        constexpr bool operator !=(const Material &other) const noexcept {
            return !(*this == other);
        }

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

        void setColour(const Colour &c) noexcept;
        void setAmbient(double a) noexcept;
        void setDiffuse(double d) noexcept;
        void setSpecular(double s) noexcept;
        void setShininess(double s) noexcept;

        constexpr bool operator==(const Material &other) const {
            return colour == other.colour
                && ALMOST_EQUALS(ambient, other.ambient)
                && ALMOST_EQUALS(diffuse, other.diffuse)
                && ALMOST_EQUALS(specular, other.specular)
                && ALMOST_EQUALS(shininess, other.shininess);
        }

        constexpr static Colour DEFAULT_COLOUR    = predefined_colours::white;
        constexpr static double DEFAULT_AMBIENT   = 0.1;
        constexpr static double DEFAULT_DIFFUSE   = 0.9;
        constexpr static double DEFAULT_SPECULAR  = 0.9;
        constexpr static double DEFAULT_SHININESS = 200;

        // Calculate the lighting of this material.
        Colour lighting(const PointLight &light, const Tuple &point, const Tuple &eyev, const Tuple &normalv) const noexcept;
    };
}