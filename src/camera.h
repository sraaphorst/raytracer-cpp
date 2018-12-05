/**
 * camera.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "affine_transform.h"

namespace raytracer::impl {
    class Ray;
}

namespace raytracer {
    class Canvas;
    class World;

    class Camera final {
        size_t hsize;
        size_t vsize;
        double fov;
        Transformation transformation;
        double half_width;
        double half_height;
        double pixel_size;

    public:
        Camera(size_t hsize, size_t vsize, double fov,
                const Transformation &transformation = predefined_matrices::I<double, 4>);
        Camera(const Camera&) = default;
        Camera(Camera&&) = default;
        Camera &operator=(const Camera&) = default;

        bool operator==(const Camera&) const noexcept;

        int getHSize() const noexcept;
        int getVSize() const noexcept;
        double getFOV() const noexcept;
        const Transformation &getTransformation() const noexcept;

        double getHalfWidth() const noexcept;
        double getHalfHeight() const noexcept;
        double getPixelSize() const noexcept;

        const impl::Ray ray_for_pixel(double px, double py) const noexcept;

        const Canvas render(const World &w) const noexcept;
    };
}




