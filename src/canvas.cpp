/**
 * canvas.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "canvas.h"

namespace raytracer {
    Canvas::Canvas(int width, int height): width{width}, height{height},
        pixels{height, std::vector<Colour>{width, predefined_colours::black}} {}
}