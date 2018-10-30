/**
 * material.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

/**
 * TODO:constexpr If we want to make this constexpr, we'll need to implement pow as a constexpr function.
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
#include <memory>

#include "pointlight.h"
#include "vec.h"

namespace raytracer {
    class Pattern;

    namespace shapes {
        class Shape;
    }

    class Material final {
    private:
        std::shared_ptr<Pattern> pattern;
        double ambient;
        double diffuse;
        double specular;
        double shininess;
        double reflectivity;
        double transparency;
        double refractive_index;

    public:
        /// Default material is just solid white.
        Material() noexcept;

        /// Keep this for backward compatibility, and convert the colour into a SolidPattern.
        Material(const Colour &colour,
                 double = DEFAULT_AMBIENT,
                 double = DEFAULT_DIFFUSE,
                 double = DEFAULT_SPECULAR,
                 double = DEFAULT_SHININESS,
                 double = DEFAULT_REFLECTIVITY,
                 double = DEFAULT_TRANSPARENCY,
                 double = DEFAULT_REFRACTIVE_INDEX) noexcept;

        Material(const std::shared_ptr<Pattern> &pattern,
                 double = DEFAULT_AMBIENT,
                 double = DEFAULT_DIFFUSE,
                 double = DEFAULT_SPECULAR,
                 double = DEFAULT_SHININESS,
                 double = DEFAULT_REFLECTIVITY,
                 double = DEFAULT_TRANSPARENCY,
                 double = DEFAULT_REFRACTIVE_INDEX) noexcept;

        Material(const Material&) = default;
        Material(Material&&) = default;
        Material &operator=(const Material&) = default;
        Material &operator=(Material&&) = default;

        bool operator==(const Material &other) const;
        bool operator!=(const Material &other) const noexcept;

        const std::shared_ptr<Pattern> &getPattern() const noexcept;
        double getAmbient() const noexcept;
        double getDiffuse() const noexcept;
        double getSpecular() const noexcept;
        double getShininess() const noexcept;
        double getReflectivity() const noexcept;
        double getTransparency() const noexcept;
        double getRefractiveIndex() const noexcept;

        void setPattern(const std::shared_ptr<Pattern>&) noexcept;
        void setAmbient(double) noexcept;
        void setDiffuse(double) noexcept;
        void setSpecular(double) noexcept;
        void setShininess(double) noexcept;
        void setReflectivity(double) noexcept;
        void setTransparency(double) noexcept;
        void setRefractiveIndex(double) noexcept;


        constexpr static Colour DEFAULT_COLOUR           = predefined_colours::white;
        constexpr static double DEFAULT_AMBIENT          =   0.1;
        constexpr static double DEFAULT_DIFFUSE          =   0.9;
        constexpr static double DEFAULT_SPECULAR         =   0.9;
        constexpr static double DEFAULT_SHININESS        = 200;
        constexpr static double DEFAULT_REFLECTIVITY     =   0;
        constexpr static double DEFAULT_TRANSPARENCY     =   0;
        constexpr static double DEFAULT_REFRACTIVE_INDEX =   1;

        // Calculate the lighting of this material.
        Colour lighting(const PointLight &light,
                        const std::shared_ptr<const shapes::Shape> &shape,
                        const Tuple &point,
                        const Tuple &eyev,
                        const Tuple &normalv,
                        bool in_shadow) const noexcept;
    };
}