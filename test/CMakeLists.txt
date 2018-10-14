cmake_minimum_required(VERSION 3.9)
project(raytracer_test)

# Enables testing from this directory downwards.
enable_testing(true)

# Prepare "Catch" library for other executables.
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Make main executable that will be linked with other tests.
set(TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests Catch)

# Create a list of all tests
set(TEST_LIST
        TestCamera
        TestCanvas
        TestColour
        TestIntersection
        TestLights
        TestMaterial
        TestMath
        TestMatrix
        TestRay
        TestShape
        TestSphere
        TestTransform
        TestTuple
        TestVariantOps
        TestWorld
        )

foreach(testname ${TEST_LIST})
    set(testfile ${testname}.cpp)
#    message("Preparing to make ${testname} with ${testfile}")
    add_executable(${testname} ${testfile} ${TEST_SOURCES})
    target_link_libraries(${testname} Catch raytracer)
endforeach()
