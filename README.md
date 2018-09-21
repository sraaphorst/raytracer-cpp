# Compile-time Ray Tracer

Compile-time Implementation of a ray tracer using Jamis Buck's book, The Ray Tracer Challenge:

https://pragprog.com/book/jbtracer/the-ray-tracer-challenge


My goal is to attempt to make the entire scene render at compile-time, with the primary motivation to teach myself more about C++ template metaprogramming. Thus far, I have written compile-time vector and matrix operations, as well as a compile-time mixed-type array implementation using `std::variant` to simulate static polymorphism so as to be able to work with arrays of different objects with same-signatured methods, which will be necessary.

Note that the current code will not compile under clang, and requires GCC in C++17 mode.
