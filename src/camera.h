/**
 * camera.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "affine_transform.h"

namespace raytracer {
    class Canvas;
    class Ray;
    class World;

    class Camera final {
        int hsize;
        int vsize;
        double fov;
        Transformation transformation;
        double half_width;
        double half_height;
        double pixel_size;

    public:
        Camera(int hsize, int vsize, double fov,
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

        const Ray ray_for_pixel(double px, double py) const noexcept;

        const Canvas render(const World &w) const noexcept;
    };
}




