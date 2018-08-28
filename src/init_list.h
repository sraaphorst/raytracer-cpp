/**
 * init_list.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Create an initializer_list with square brackets to access elements.
 */

#pragma once

#include <initializer_list>

namespace raytracer {
    template<class T>
    struct init_list_with_square_brackets {
        const std::initializer_list <T> &lst;

        constexpr init_list_with_square_brackets(const std::initializer_list <T> &lst) : lst(lst) {}

        constexpr T operator[](const unsigned int index) {
            return *(lst.begin() + index);
        }
    };

    template<class T>
    constexpr init_list_with_square_brackets<T> idx(const std::initializer_list <T> &list) {
        return init_list_with_square_brackets<T>(list);
    }
}


