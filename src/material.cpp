/**
 * material.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "material.h"
#include "pointlight.h"
#include "vector.h"

namespace raytracer {
    void Material::setColour(const Colour &c) noexcept {
        colour = c;
    }
    void Material::setAmbient(double a) noexcept {
        ambient = a;
    }
    void Material::setDiffuse(double d) noexcept {
        diffuse = d;
    }
    void Material::setSpecular(double s) noexcept {
        specular = s;
    }
    void Material::setShininess(double s) noexcept {
        shininess = s;
    }

    Colour Material::lighting(const PointLight &light, const Tuple &point,
                    const Tuple &eyev, const Tuple &normalv) const {
        const auto effective_colour = colour * light.getIntensity();
        const auto lightv = (light.getPosition() - point).normalize();
        const auto ambient_component = ambient * effective_colour;
        const auto light_dot_normal = lightv.dot_product(normalv);

        const auto diffuse_component = (light_dot_normal < 0)
                                       ? predefined_colours::black
                                       : diffuse * effective_colour * light_dot_normal;

        const auto reflectv = -lightv.reflect(normalv);
        const auto reflect_dot_eye = std::pow(reflectv.dot_product(eyev), shininess);

        const auto specular_component = (light_dot_normal < 0 || reflect_dot_eye <= 0)
                                        ? predefined_colours::black
                                        : reflect_dot_eye * specular * light.getIntensity();

        return ambient_component + diffuse_component + specular_component;
    }
}