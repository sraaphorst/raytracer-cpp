# Ray Tracer

**Current status:** In progress.

This is my implementation of a ray tracer using Jamis Buck's book, The Ray Tracer Challenge:

https://pragprog.com/book/jbtracer/the-ray-tracer-challenge

My goal was to attempt to make the entire scene render at compile-time, with the primary motivation to teach myself more about C++ template metaprogramming. Thus far, I have written compile-time vector and matrix operations, as well as a compile-time mixed-type array implementation using extendable `std::variant` to simulate static polymorphism so as to be able to work with arrays of different objects with same-signatured methods, which will be necessary. (I don't think visitors could be used for this because of the desire to allow user plug-in types. I will also still need a number of `constexpr` math functions, such as `pow(double base, double exponent)`, amongst other things.

Note that the current code may not compile under clang, and requires GCC in C++17 mode. It also requires cmake 3.12, as it uses list transformers.

**NOTE:** Due to time constraints (as a technical reviewer of "The Ray Tracer Challenge"), I have decided to implement the ray tracer to work at run-time instead of entirely at compile-time. In the future, I may plan to branch off this and modify it to render at compile time.

A scene rendered entirely out of cubes:

![sphere](output/table.png)

Reflection and refraction:

![scene](output/reflect_refract.png)
