/**
 * material.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <memory>

#include "material.h"
#include "pattern.h"
#include "pointlight.h"
#include "solidpattern.h"
#include "vector.h"

namespace raytracer {
    Material::Material() noexcept:
            pattern{std::make_shared<SolidPattern>(DEFAULT_COLOUR)},
            ambient{DEFAULT_AMBIENT},
            diffuse{DEFAULT_DIFFUSE},
            specular{DEFAULT_SPECULAR},
            shininess{DEFAULT_SHININESS} {}

    Material::Material(const Colour &colour,
                       double ambient,
                       double diffuse,
                       double specular,
                       double shininess) noexcept:
            pattern{std::make_shared<SolidPattern>(colour)},
            ambient{ambient},
            diffuse{diffuse},
            specular{specular},
            shininess{shininess} {}

    Material::Material(const std::shared_ptr<Pattern> &pattern,
                       double ambient,
                       double diffuse,
                       double specular,
                       double shininess) noexcept:
            pattern{pattern},
            ambient{ambient},
            diffuse{diffuse},
            specular{specular},
            shininess{shininess} {}

    bool Material::operator==(const Material &other) const {
        return *pattern == *other.pattern
               && ALMOST_EQUALS(ambient, other.ambient)
               && ALMOST_EQUALS(diffuse, other.diffuse)
               && ALMOST_EQUALS(specular, other.specular)
               && ALMOST_EQUALS(shininess, other.shininess);
    }

    bool Material::operator!=(const Material &other) const noexcept {
        return *this != other;
    }

    const std::shared_ptr<Pattern> &Material::getPattern() const noexcept {
        return pattern;
    }
    double Material::getAmbient() const noexcept {
        return ambient;
    }
    double Material::getDiffuse() const noexcept {
        return diffuse;
    }
    double Material::getSpecular() const noexcept {
        return specular;
    }
    double Material::getShininess() const noexcept {
        return shininess;
    }

    void Material::setPattern(const std::shared_ptr<Pattern> &p) noexcept {
        pattern = p;
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
                    const Tuple &eyev, const Tuple &normalv, bool in_shadow) const noexcept {
        const auto colour = pattern->colour_at(point);
        const auto effective_colour = colour * light.getIntensity();
        const auto lightv = (light.getPosition() - point).normalize();
        const auto ambient_component = ambient * effective_colour;

        // If the point is in shadow, we only use the ambient component.
        if (in_shadow)
            return ambient_component;

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