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

        template<typename T>
        explicit Shape(T&& t): transformation{t}, material{Material{}} {}

        template<typename T, typename S>
        Shape(T&& transformation, S&& material) noexcept: transformation{transformation}, material{material} {}

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

        /**
         * Convert the ray to object space and then pass it to the concrete implementation of local_intersect,
         * which is subclass-dependent.
         */
        const std::vector<Intersection> intersect(const Ray&) const noexcept;

        /**
         * Takes a point and transforms it to object space. It is then passed to localNormalAt, which is
         * subclass-dependent. The normal vector is then translated back to world space and returned.
         */
        const Tuple normalAt(const Tuple &p) const noexcept;

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
        virtual const std::vector<Intersection> localIntersection(const Ray &r) const noexcept = 0;

        /**
         * The normalAt method transforms the point to object space and passes it to this method, which
         * should comprise the concrete implementation of calculating the normal vector at the point for the
         * implemented Shape. The normalAt method then translates it back to world space.
         */
        virtual const Tuple localNormalAt(const Tuple&) const noexcept = 0;


        Transformation transformation;
        Material material;
    };
}
