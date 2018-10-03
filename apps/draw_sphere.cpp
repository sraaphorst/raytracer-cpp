/**
 * draw_sphere.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <fstream>

#include "canvas.h"
#include "ray.h"
#include "sphere.h"
#include "vector.h"

using namespace raytracer;

int main() {
    constexpr int canvas_pixels = 100;

    // Start the ray at z = -5.
    const auto ray_origin = make_point(0, 0, -5);

    // Put the wall at z = 10.
    constexpr double wall_z = 10;
    constexpr double wall_size = 7;

    // Determine the pixel size.
    constexpr double pixel_size = wall_size / canvas_pixels;

    // As sphere is centered at origin, this variable describes min / max
    // x and y values.
    constexpr double half = wall_size / 2;

    Canvas<canvas_pixels, canvas_pixels> c{};
    colour_ptr_t colour_ptr{new Colour{1, 0, 0}};

    Sphere s;
    s.setTransformation(scale(0.5, 1, 1).andThen(rotation_z(M_PI_4)));

    // For each row of pixels in the canvas:
    for (auto y = 0; y < canvas_pixels; ++y) {
        // Compute the world y coordinate (top = +half, bottom = -half).
        const auto world_y = half - pixel_size * y;

        // For each pixel in the row:
        for (auto x = 0; x < canvas_pixels; ++x) {
            // Compute the world x coordinate (left = -half, right = +half).
            const auto world_x = -half + pixel_size * x;

            // Describe the point on the wall that the ray will target.
            const auto position = make_point(world_x, world_y, wall_z);

            const Ray r{ray_origin, (position - ray_origin).normalize()};
            const auto xs = s.intersect(r);
            const auto hit = Intersection::hit(xs);
            if (hit.has_value()) {
                c[x][y] = colour_ptr;
            }
        }
    }

    std::ofstream out("sphere.ppm");
    out << c;
    out.close();
}