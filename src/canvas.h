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

#include "colour.h"

namespace raytracer {
    using colour_ptr_t = std::shared_ptr<const colour>;

    template<size_t width, size_t height>
    class canvas final {
    private:
        using col  = std::array<colour_ptr_t, height>;
        using grid = std::array<col, width>;

        std::shared_ptr<const colour> black = std::make_shared<colour>(0, 0, 0);

        /// Use shared_ptrs so that we can reuse colours, like black on initialization.
        grid pixels;

    public:
        canvas() {
            for (int i=0; i < width; ++i)
                for (int j=0; j < height; ++j)
                    pixels[i][j] = std::shared_ptr<const colour>(black);
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
            ostr << "P3\n" << width << ' ' << height << '\n' << colour::maxvalue << '\n';
            for (auto j=0; j < height; ++j) {
                for (auto i=0; i < width; ++i) {
                    for (auto rgb = 0; rgb < 3; ++rgb) {
                        int cval = (int) ((*c[i][j])[rgb] * colour::maxvalue + 0.5);
                        auto val = std::max(0, std::min(cval, colour::maxvalue));
                        ostr << val << (i == width - 1 && rgb == 2 ? "" : " ");
                    }
                    if (i == width - 1 && j < height - 1)
                        ostr << '\n';
                }
            }
            return ostr;
        }
    };
}



