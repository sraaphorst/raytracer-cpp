/**
 * meta.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generic functions used in metaprogramming.
 */

#pragma once

namespace raytracer::meta {
    /**
     * Determine if type T occurs in the types in List.
     */
    template<typename T, typename... List>
    struct contains: std::true_type {};

    template<typename T, typename Head, typename... Tail>
    struct contains<T, Head, Tail...>
            : std::conditional<std::is_same<T, Head>::value,
                    std::true_type,
                    contains<T, Tail...>
            >::type {};

    template <typename T>
    struct contains<T>: std::false_type {};

}