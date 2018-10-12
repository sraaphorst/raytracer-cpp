/**
 * canvas.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <cmath>
#include <cstddef>
#include <memory>
#include <ostream>
#include <vector>

#include "constmath.h"
#include "vector.h"

namespace raytracer {
    class Canvas final {
        const int width;
        const int height;

        using col  = std::vector<Colour>;
        using grid = std::vector<col>;

        /// Use shared_ptrs so that we can reuse colours, like black on initialization.
        grid pixels;

    public:
        Canvas(int width, int height);

        /// This is mutable, so allow access to contents through indexing.
        col& operator[](const int idx) {
            return pixels[idx];
        }

        const col& operator[](const int idx) const {
            return pixels[idx];
        }

        const size_t getWidth() const noexcept {
            return width;
        }

        const size_t getHeight() const noexcept {
            return height;
        }

        /// Create a stream representing this as a PPM file.
        friend std::ostream &operator<<(std::ostream &ostr, const Canvas &c) {
            ostr << "P3\n" << c.width << ' ' << c.height << '\n' << colour_constants::maxvalue << '\n';

            int linewidth = 0;
            for (auto j=0; j < c.height; ++j) {
                bool first = true;

                for (auto i=0; i < c.width; ++i) {
                    for (auto rgb = 0; rgb < 3; ++rgb) {
                        auto cval = (int) (c[i][j][rgb] * colour_constants::maxvalue + 0.5);
                        auto val = std::max(0, std::min(cval, colour_constants::maxvalue));

                        // Constrain lines to 70 characters as per PPM specifications.
                        auto valwidth = numDigits(val);
                        if (linewidth + 1 + valwidth > 70) {
                            ostr << '\n';
                            linewidth = 0;
                            first = true;
                        }

                        // Separate by spaces.
                        if (first)
                            first = false;
                        else {
                            ostr << ' ';
                            ++linewidth;
                        }

                        ostr << val;
                        linewidth += valwidth;
                    }
                }

                // We always want a newline at the end of the output as per PPM specification.
                ostr << '\n';
                linewidth = 0;
            }
            return ostr;
        }
    };
}
