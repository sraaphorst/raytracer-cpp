/**
 * camera.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "camera.h"
#include "canvas.h"
#include "common.h"
#include "constmath.h"
#include "ray.h"
#include "vector.h"
#include "world.h"

namespace raytracer {
    Camera::Camera(int hsize, int vsize, double fov, const Transformation &transformation) :
            hsize{hsize}, vsize{vsize}, fov{fov}, transformation{transformation} {
        const auto half_view = tanc(fov / 2);
        const auto aspect    = (double) hsize / vsize;
        if (aspect >= 1) {
            half_width = half_view;
            half_height = half_view / aspect;
        } else {
            half_width = half_view * aspect;
            half_height = half_view;
        }

        pixel_size = (half_width * 2) / hsize;
    }

    bool Camera::operator==(const Camera &other) const noexcept {
        return hsize == other.hsize
            && vsize == other.vsize
            && ALMOST_EQUALS(fov, other.fov)
            && transformation == other.transformation;
    }

    int Camera::getHSize() const noexcept {
        return hsize;
    }

    int Camera::getVSize() const noexcept {
        return vsize;
    }

    double Camera::getFOV() const noexcept {
        return fov;
    }

    const Transformation &Camera::getTransformation() const noexcept {
        return transformation;
    }

    double Camera::getHalfWidth() const noexcept {
        return half_width;
    }

    double Camera::getHalfHeight() const noexcept {
        return half_height;
    }

    double Camera::getPixelSize() const noexcept {
        return pixel_size;
    }

    const Ray Camera::ray_for_pixel(double px, double py) const noexcept {
        const auto xoffset = (px + 0.5) * pixel_size;
        const auto yoffset = (py + 0.5) * pixel_size;

        const auto world_x = half_width - xoffset;
        const auto world_y = half_height - yoffset;

        const auto inv    = transformation.invert();
        const auto pixel  = inv * make_point(world_x, world_y, -1);
        const auto origin = inv * make_point(0, 0, 0);

        const auto direction = (pixel - origin).normalize();
        return Ray{origin, direction};
    }

    const Canvas Camera::render(const World &w) const noexcept {
        auto image = Canvas{hsize, vsize};

        for (size_t y = 0; y < vsize; ++y)
            for (size_t x = 0; x < hsize; ++x) {
                const auto ray = ray_for_pixel(x, y);
                const auto colour = w.colourAt(ray);
                image[x][y] = colour;
            }

        return image;
    }
}