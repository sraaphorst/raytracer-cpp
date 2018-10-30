/**
 * clock.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Draw a simplistic clock with pixels representing the hour markers.
 */

#include <chrono>
#include <ctime>
#include <fstream>
#include <functional>
#include <memory>

#include "affine_transform.h"
#include "canvas.h"
#include "constmath.h"
#include "vec.h"

using namespace raytracer;

int main() {
    constexpr int size  = 300;
    constexpr int border = 50;

    Colour colour{1, 1, 0};
    Canvas c{size, size};

    // Calculate the rotation needed to advance one hour.
    constexpr auto rot = math_constants::pi<> / 6;

    // Scale to the desired height.
    constexpr auto sz = (size - 2 * border) / 2;
    constexpr auto sc = scale(sz, sz, 0);

    // Calculate the translation to get to the middle of the canvas.
    constexpr auto tr = translation(size / 2, size / 2, 0);

    // Start with a point pointing straight up to 12:00.
    constexpr auto p = make_point(0, -1, 0);

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
        c[newp[tuple_constants::x]][newp[tuple_constants::y]] = colour;
    }

    // Let's add hands to show the current time.
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto now2 = std::localtime(&now);
    int hour = now2->tm_hour % 12;
    int min  = now2->tm_min;

    // Colour of the hour and minute hand.
    const auto hour_colour = predefined_colours::red;
    const auto min_colour  = make_colour(0.5, 0.5, 1);

    // Angle per minute.
    constexpr auto minuteangle = math_constants::pi<> / 30;

    // Draw the hour hand.
    for (int i = -10; i < sz - 20; ++i) {
        auto newp = rotation_z((hour + min / 60.0) * rot)
                .andThen(scale(i, i, i))
                .andThen(tr) * p;
        c[newp[tuple_constants::x]][newp[tuple_constants::y]] = hour_colour;
    }

    // Draw the minute hand.
    for (int i = -10; i < sz; ++i) {
        auto newp = rotation_z(min * minuteangle)
                .andThen(scale(i, i, i))
                .andThen(tr) * p;
        c[newp[tuple_constants::x]][newp[tuple_constants::y]] = min_colour;
    }

    std::ofstream out("clock.ppm");
    out << c;
    out.close();
}
