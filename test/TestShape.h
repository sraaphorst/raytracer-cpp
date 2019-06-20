/**
 * TestShape.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <memory>
#include <vector>

#include "bounding_box.h"
#include "affine_transform.h"
#include "intersection.h"
#include "material.h"
#include "matrix.h"
#include "ray.h"
#include "shape.h"
#include "vec.h"

/**
 * This is a class that implements just enough of Shape to be a concrete implementation,
 * so as to be used to isolate and test the common characteristics of all subclasses of Shape.
 */
class TestShape final: public raytracer::shapes::Shape {

public:
    // Saves the ray passed to localIntersection to make sure the proper transformation happens.
    raytracer::impl::Ray saved_ray;

public:
    explicit TestShape(dummy d): Shape{d} {}

    static std::shared_ptr<TestShape> createTestShape() {
        std::shared_ptr<TestShape> ts = std::make_shared<TestShape>(dummy{});
        registerInstance(ts);
        return ts;
    }

    /// Get a bounding box.
    raytracer::impl::BoundingBox boundsOf() const override {
        return raytracer::impl::BoundingBox{raytracer::make_point(-1, -1, -1), raytracer::make_point(1, 1, 1)};
    }

private:
    /// Since this has no actual form, it just saves the ray to make sure it is properly translated to object space.
    const std::vector<raytracer::impl::Intersection> localIntersection(const raytracer::impl::Ray &r)
            const noexcept override {
        // This is an exceptional situation.
        const_cast<TestShape*>(this)->saved_ray = r;
        return {};
    }

    const raytracer::Tuple localNormalAt(const raytracer::Tuple &point) const noexcept override {
        return point - raytracer::predefined_tuples::zero_point;
    }
};