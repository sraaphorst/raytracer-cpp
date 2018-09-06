/**
 * clock.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Draw a simplistic clock with pixels representing the hour markers.
 */

#include <fstream>
#include <functional>
#include <memory>

#include "affine_transform.h"
#include "canvas.h"
#include "constmath.h"
#include "vector.h"

using namespace raytracer;

int main() {
    constexpr int width  = 300;
    constexpr int height = 300;
    constexpr int border = 50;

    colour_ptr_t colour_ptr{new Colour{1, 1, 0}};
    Canvas<width, height> c{};

    // Calculate the rotation needed to advance one hour.
    constexpr auto rot = math_constants::pi<> / 6;

    // Scale to the desired height.
    constexpr auto sc = scale((width - 2 * border) / 2, (height - 2 * border) / 2, 0);

    // Calculate the translation to get to the middle of the canvas.
    constexpr auto tr = translation(width / 2, height / 2, 0);

    // Start with a point pointing straight up to 12:00.
    constexpr auto p = make_point(0, 1, 0);

//    // The first two techniques build up rotations.
//    // In the first, we use pointers, since Matrix is immutable.
//    // In the second, we use a recursive lambda.
//    auto transform = std::unique_ptr<Transformation>{new Transformation{rot.andThen(sc).andThen(tr)}};
//    for (size_t i = 0; i < 12; ++i) {
//        auto newp = *transform * p;
//        c[newp[tuple_constants::x]][newp[tuple_constants::y]] = colour_ptr;
//        transform = std::unique_ptr<Transformation>{new Transformation{rot.andThen(*transform)}};
//    }

//    // Using a recursive lambda.
//    std::function<Transformation(size_t, Transformation)> runner = [&] (size_t iterations, Transformation transformation){
//        if (iterations == 0)
//            return transformation;
//        auto newp = transformation * p;
//        c[newp[tuple_constants::x]][newp[tuple_constants::y]] = colour_ptr;
//        return runner(iterations - 1, rot.andThen(transformation));
//    };
//    runner(12, sc.andThen(tr));


    for (size_t i = 0; i < 12; ++i) {
        auto newp = rotation_z(i * rot).andThen(sc).andThen(tr) * p;
        c[newp[tuple_constants::x]][newp[tuple_constants::y]] = colour_ptr;
    }

    std::ofstream out("clock.ppm");
    out << c;
    out.close();
}
