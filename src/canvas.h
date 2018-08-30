/**
 * canvas.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <array>
#include <cmath>
#include <cstddef>
#include <memory>
#include <ostream>

#include "constmath.h"
#include "colour.h"

namespace raytracer {
    using colour_ptr_t = std::shared_ptr<const Colour>;

    template<size_t width, size_t height>
    class canvas final {
    private:
        using col  = std::array<colour_ptr_t, height>;
        using grid = std::array<col, width>;

        std::shared_ptr<const Colour> black = std::make_shared<const Colour>(0, 0, 0);

        /// Use shared_ptrs so that we can reuse colours, like black on initialization.
        grid pixels;

    public:
        canvas() {
            for (int i=0; i < width; ++i)
                for (int j=0; j < height; ++j)
                    pixels[i][j] = std::shared_ptr<const Colour>(black);
        }

        /// This is mutable, so allow access to contents through indexing.
        col& operator[](const int idx) {
            return pixels[idx];
        }

        const col& operator[](const int idx) const {
            return pixels[idx];
        }

        /// Create a stream representing this as a PPM file.
        friend std::ostream &operator<<(std::ostream &ostr, const canvas<width, height> &c) {
            ostr << "P3\n" << width << ' ' << height << '\n' << Colour::maxvalue << '\n';

            int linewidth = 0;
            for (auto j=0; j < height; ++j) {
                bool first = true;

                for (auto i=0; i < width; ++i) {
                    for (auto rgb = 0; rgb < 3; ++rgb) {
                        auto cval = (int) ((*c[i][j])[rgb] * Colour::maxvalue + 0.5);
                        auto val = std::max(0, std::min(cval, Colour::maxvalue));

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
