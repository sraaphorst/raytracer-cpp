/**
 * shape.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <memory>
#include <tuple>
#include <vector>

#include "affine_transform.h"
#include "shape.h"
#include "intersection.h"
#include "material.h"
#include "matrix.h"
#include "ray.h"

using namespace raytracer;
using namespace raytracer::impl;

namespace raytracer::shapes {
    Shape::Shape(dummy d) noexcept:
        InstanceManager{d},
        transformation{predefined_matrices::I<double, 4>},
        material{std::make_shared<Material>()} {}

    bool Shape::operator==(const Shape &other) const noexcept {
        return typeid(*this) == typeid(other)
               && transformation == other.transformation
               && *material == *other.material
               && doCompare(other);
    }

    bool Shape::operator!=(const Shape &other) const noexcept {
        return !(*this == other);
    }

    const Transformation &Shape::getTransformation() const {
        return transformation;
    }

    Transformation &Shape::getTransformation() {
        return transformation;
    }

    void Shape::setTransformation(Transformation&& t) {
        transformation = std::move(t);
    }

    void Shape::setTransformation(const Transformation &t) {
        transformation = t;
    }

    void Shape::setTransformation(Transformation &t) {
        transformation = t;
    }

    const std::shared_ptr<Material> &Shape::getMaterial() const {
        return material;
    }

    std::shared_ptr<Material> &Shape::getMaterial() {
        return material;
    }

    void Shape::setMaterial(std::shared_ptr<Material> &&m) {
        material = std::move(m);
    }

    void Shape::setMaterial(const std::shared_ptr<Material> &m) {
        material = m;
    }

    void Shape::setMaterial(std::shared_ptr<Material> &m) {
        material = m;
    }

    const std::vector<Intersection> Shape::intersect(const Ray &r0) const noexcept {
        // Transform the ray to object space.
        const Ray r = r0.transform(getTransformation().invert());

        // Return the shape-dependent implementation results.
        return localIntersection(r);
    }

    const Tuple Shape::normalAt(const Tuple &point) const noexcept {
        // Transform the point to object space.
        const auto local_point = transformation.invert() * point;
        const auto local_normal = localNormalAt(local_point);
        const auto world_normal = transformation.invert().transpose() * local_normal;

        // w could have a value after this, so get rid of it.
        return make_vector(world_normal[tuple_constants::x],
                           world_normal[tuple_constants::y],
                           world_normal[tuple_constants::z]).normalize();
    }
}