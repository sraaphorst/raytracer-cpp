/**
 * shape.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <memory>
#include <tuple>
#include <vector>

#include "affine_transform.h"
#include "instance_manager.h"
#include "material.h"
#include "vec.h"

namespace raytracer::impl {
    class Intersection;
    class Ray;
}

namespace raytracer::shapes {
    /**
     * Note that there is some ghastliness in how the intersections are created right now, since they need a
     * shared_ptr to this. We could use std::enabled_shared_from_this, and then use make_shared_from_this(),
     * but then we need to have a shared_ptr to this stored somewhere as a prerequisite to doing so.
     *
     * So right now, we duplicate the objects, which is horrible.
     * TODO: Fix this. All Shapes should be factory managed and use shared_ptr.
     */
    class Shape: public impl::InstanceManager, public std::enable_shared_from_this<Shape> {
    protected:
        Transformation transformation;
        Transformation transformationInverse;
        Transformation transformationInverseTranspose;
        bool casts_shadow;
        std::shared_ptr<Material> material;

    public:
        explicit Shape(dummy d) noexcept;

        /**
         * Compare type compatibility, transformation and material, and then invoke the concrete implementation,
         * doCompare, which is subclass-dependent. Override if necessary.
         */
        bool operator==(const Shape&) const noexcept;
        bool operator!=(const Shape&) const noexcept;

        const Transformation &getTransformation() const noexcept;
        const Transformation &getTransformationInverse() const noexcept;
        void setTransformation(Transformation&&);
        void setTransformation(const Transformation&);
        void setTransformation(Transformation&);

        const std::shared_ptr<Material> &getMaterial() const;
        std::shared_ptr<Material> &getMaterial();
        void setMaterial(std::shared_ptr<Material>&&);
        void setMaterial(const std::shared_ptr<Material>&);
        void setMaterial(std::shared_ptr<Material>&);

        bool castsShadow() const noexcept;
        void setCastsShadow(bool s) noexcept;

        /**
         * Convert the ray to object space and then pass it to the concrete implementation of local_intersect,
         * which is subclass-dependent.
         */
        const std::vector<impl::Intersection> intersect(const impl::Ray&) const noexcept;

        /**
         * Takes a point and transforms it to object space. It is then passed to localNormalAt, which is
         * subclass-dependent. The normal vector is then translated back to world space and returned.
         */
        const Tuple normalAt(const Tuple &p) const noexcept;

    protected:
        /**
         * Any additional implementation for equality comparison in subclasses, should be implemented here.
         */
        virtual bool doCompare(const Shape&) const noexcept {
            return true;
        }

        /**
         * The intersect method transforms the ray to object space and passes it to this method, which
         * should comprise the concrete implementation of calculating the intersections with the implemented Shape.
         */
        virtual const std::vector<impl::Intersection> localIntersection(const impl::Ray &r)
            const noexcept = 0;

        /**
         * The normalAt method transforms the point to object space and passes it to this method, which
         * should comprise the concrete implementation of calculating the normal vector at the point for the
         * implemented Shape. The normalAt method then translates it back to world space.
         */
        virtual const Tuple localNormalAt(const Tuple&) const noexcept = 0;
    };
}
