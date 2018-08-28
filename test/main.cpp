/**
 * main.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Create a main function for Catch2 tests, which will link to this file.
 */

// Define these for Travis, as it has trouble with std::uncaught_exceptions.
#define CATCH_INTERNAL_CONFIG_NO_CPP17_UNCAUGHT_EXCEPTIONS
#define CATCH_CONFIG_NO_CPP17_UNCAUGHT_EXCEPTIONS
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
