/**
 * material.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

namespace raytracer {
    class Material final {
    private:
        const double ambient;
        const double diffuse;
        const double specular;
        const double shininess;

    public:
        constexpr Material(double ambient, double diffuse, double specular, double shininess) noexcept:
            ambient{ambient}, diffuse{diffuse}, specular{specular}, shininess{shininess} {}
        constexpr Material() noexcept: Material{0, 0, 0, 0} {}
        constexpr Material(const Material&) = default;
        constexpr Material(Material&&) = default;

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
    };
}