/**
 * shape.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <vector>

#include "affine_transform.h"
#include "material.h"
#include "vector.h"


namespace raytracer {
    class Intersection;
    class Ray;

    class Shape {
    public:
        Shape() noexcept;

        template<typename T, typename S>
        Shape(T&& transformation, S&& material) noexcept: transformation{transformation}, material{material} {}

        /**
         * Convert the ray to object space and then pass it to the concrete implementation of local_intersect,
         * which is subbclass-dependent.
         */
        const std::vector<Intersection> intersect(const Ray&) const noexcept;

        /**
         * Compare type compatibility, transformation and material, and then invoke the concrete implementation,
         * doCompare, which is subclass-dependent. Override if necessary.
         */
        bool operator==(const Shape&) const noexcept;
        bool operator!=(const Shape&) const noexcept;

        const Transformation &getTransformation() const {
            return transformation;
        }

        Transformation &getTransformation() {
            return transformation;
        }

        void setTransformation(Transformation&& t) {
            transformation = std::move(t);
        }

        void setTransformation(const Transformation &t) {
            transformation = t;
        }

        void setTransformation(Transformation &t) {
            transformation = t;
        }

        const Material &getMaterial() const {
            return material;
        }

        Material &getMaterial() {
            return material;
        }

        void setMaterial(Material &&m) {
            material = std::move(m);
        }

        void setMaterial(const Material &m) {
            material = m;
        }

        void setMaterial(Material &m) {
            material = m;
        }

        virtual const Tuple normalAt(const Tuple &p) const noexcept = 0;

    protected:
        /**
         * Any additional implementation for equality comparison in subclasses, should be implemented here.
         */
        virtual bool doCompare(const Shape &other) const noexcept {
            return true;
        }

        /**
         * The intersect method transforms the ray to object space and passes it to this method, which
         * should comprise the concrete implementation of calculating the intersections with the implemented Shape.
         */
        virtual const std::vector<Intersection> local_intersect(const Ray &r) const noexcept = 0;

        Transformation transformation;
        Material material;
    };
}
